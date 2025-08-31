#pragma once
#include <algorithm>    // std::reverse
#include <memory>       // std::shared_ptr
#include <type_traits>  // is_function_v
#include <vector>

#include "algorithm.hpp"
#include "cubie_cube.hpp"

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

    bool is_on_inverse() const { return seq.inv_flag; }

    template <typename Initializer, typename Solver>
    auto expand(const Initializer &initialize, const Solver &solve,
                const unsigned max_depth, const unsigned slackness,
                const bool niss = true) {
        // init one root, two if we solve inverse as well (niss = true)
        auto roots = initialize(state, niss);
        if (is_on_inverse() && niss) {
            // if we are already on inverse, change root inverse flags
            roots[0].inverse = true;
            roots[1].inverse = false;
        }

        auto children = std::vector<StepNode::sptr>{};
        auto step_sols = solve(roots, max_depth, slackness);
        for (auto &&sol : step_sols) {
            Algorithm seq = sol->get_path();
            CubieCube copy = state;
            copy.apply(seq);
            unsigned d = depth + sol->depth;
            children.emplace_back(
                new StepNode(copy, seq, this->shared_from_this(), d));
        }

        return children;
    }
};

auto make_step_root(const CubieCube &cc) {
    return typename StepNode::sptr(
        new StepNode(cc, Algorithm(), StepNode::sptr{nullptr}, 0));
}

constexpr bool NISS = true;
constexpr bool NONISS = false;

template <typename Current, typename... Next>
auto jaap_multistep(std::vector<StepNode::sptr> &queue, unsigned slackness_left,
                    Current &step, Next &...steps) {
    // Multistep solver idea from Jaap Scherphuis
    // https://www.jaapsch.net/puzzles/compcube.htm

    std::vector<StepNode::sptr> solutions, ret;
    if constexpr (sizeof...(steps) == 0) {
        // No slackness in last step
        assert(slackness_left == 0);
        ret = step(queue, 0);
    } else if constexpr (sizeof...(steps) == 1) {
        // Penultimate step uses all the remaining slackness
        solutions = step(queue, slackness_left);
        ret = jaap_multistep(solutions, 0, steps...);
    } else {
        unsigned shortest_path_length = 100;
        for (unsigned s = 0; s <= slackness_left; ++s) {
            auto step_sol = step(queue, s);
            solutions = jaap_multistep(step_sol, slackness_left - s, steps...);
            for (auto sol : solutions) {
                if (sol->depth < shortest_path_length) {
                    ret.clear();  // clear existing solutions cause we
                    // found shorter ones
                    shortest_path_length = sol->depth;
                }
                if (sol->depth == shortest_path_length) {
                    ret.push_back(sol);
                }
            }
        }
    }
    return ret;
}