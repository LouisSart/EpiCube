#pragma once
#include <algorithm>    // std::reverse
#include <memory>       // std::shared_ptr
#include <type_traits>  // is_function_v
#include <vector>

#include "algorithm.hpp"
#include "cubie_cube.hpp"
#include "node.hpp"

struct StepNode : std::enable_shared_from_this<StepNode> {
    using sptr = std::shared_ptr<StepNode>;
    CubieCube state;
    Algorithm seq;
    sptr parent;
    unsigned depth;

    StepNode(const CubieCube &state, const Algorithm &seq = "",
             sptr parent = nullptr, unsigned depth = 0)
        : state(state), seq(seq), parent(parent), depth(depth) {}

    Skeleton get_skeleton(const std::vector<std::string> &comments) const {
        Skeleton ret;

        auto node = *this;
        while (node.parent) {
            ret.push_back(StepAlgorithm(node.seq));
            node = *node.parent;
        }
        assert(ret.size() == comments.size());
        std::reverse(ret.begin(), ret.end());
        for (size_t i = 0; i < comments.size(); ++i) {
            ret[i].comment = comments[i];
        }
        return ret;
    }

    Skeleton get_skeleton(std::vector<std::string> &&comments) const {
        return get_skeleton(comments);
    }
};

auto make_step_root(const CubieCube &cc) {
    return typename StepNode::sptr(
        new StepNode(cc, Algorithm(), StepNode::sptr{nullptr}, 0));
}

template <typename Cube, bool niss = true>
auto make_stepper(auto &expand, auto &estimate, auto &is_solved) {
    return [&expand, &estimate, &is_solved](
               std::vector<typename StepNode::sptr> &step_roots,
               unsigned max_depth, unsigned slackness) {
        std::vector<typename StepNode::sptr> ret;
        CubieCube state;
        unsigned search_depth;

        for (auto step_root : step_roots) {
            auto roots = std::vector<typename Node<Cube>::sptr>{
                make_root(Cube(step_root->state))};
            if (niss)
                roots.push_back(
                    make_root(Cube(step_root->state.get_inverse()), true));

            if (max_depth > step_root->depth) {
                search_depth = max_depth - step_root->depth;
                auto solutions = IDAstar(roots, expand, estimate, is_solved,
                                         search_depth, slackness);
                for (auto sol : solutions) {
                    state = step_root->state;
                    state.apply(
                        sol->get_path());  // apply from inverse if sol->inverse
                    ret.emplace_back(
                        new StepNode(state, sol->get_path(), step_root,
                                     step_root->depth + sol->depth));
                }
            }
        }
        return ret;
    };
}

template <typename Current, typename... Next>
auto multistep(std::vector<StepNode::sptr> &queue, unsigned max_depth,
               unsigned slackness_left, Current &step, Next &...steps) {
    // Multistep solver idea from Jaap Scherphuis
    // https://www.jaapsch.net/puzzles/compcube.htm

    std::vector<StepNode::sptr> solutions, ret;
    if constexpr (sizeof...(steps) == 0) {
        // No slackness in last step
        assert(slackness_left == 0);
        ret = step(queue, max_depth, 0);
    } else if constexpr (sizeof...(steps) == 1) {
        // Penultimate step uses all the remaining slackness
        solutions = step(queue, max_depth, slackness_left);
        ret = multistep(solutions, max_depth, 0, steps...);
    } else {
        unsigned best_depth = max_depth;
        for (unsigned s = 0; s <= slackness_left; ++s) {
            auto step_sol = step(queue, max_depth, s);
            solutions =
                multistep(step_sol, best_depth, slackness_left - s, steps...);
            for (auto sol : solutions) {
                if (sol->depth < best_depth) {
                    ret.clear();  // clear existing solutions cause we
                    // found shorter ones
                    best_depth = sol->depth;
                }
                if (sol->depth == best_depth) {
                    ret.push_back(sol);
                }
            }
        }
    }
    return ret;
}

template <typename... Steps>
auto jaap_solver(std::vector<StepNode::sptr> &queue, unsigned max_slackness,
                 Steps &...steps) {
    unsigned max_depth = 100;
    std::vector<StepNode::sptr> solutions;
    for (auto s = 0; s <= max_slackness; ++s) {
        print("Searching with", s, "moves of slackness");
        solutions = multistep(queue, max_depth, s, steps...);
        max_depth = solutions[0]->depth;
        print("solutions found at depth", max_depth);
    }
    return solutions;
}