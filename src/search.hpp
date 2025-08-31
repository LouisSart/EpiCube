#pragma once
#include <algorithm>
#include <cassert>
#include <vector>

#include "move.hpp"
#include "node.hpp"

template <typename NodePtr>
struct Solutions : public std::vector<NodePtr> {
    unsigned best_hope;

    void sort_by_depth() {
        std::sort(this->begin(), this->end(),
                  [](const NodePtr node1, const NodePtr node2) {
                      return (node1->depth < node2->depth);
                  });
    }

    void show() const {
        for (auto node : *this) {
            node->get_path().show();
        }
    }
};

template <bool verbose = false, typename NodePtr, typename Expander,
          typename Pruner, typename SolveCheck>
Solutions<NodePtr> depth_first_search(std::vector<NodePtr> queue,
                                      const Expander &expand,
                                      const Pruner &estimate,
                                      const SolveCheck &is_solved,
                                      const unsigned max_depth = 4) {
    // Main implementation starting from any number of root states
    Solutions<NodePtr> solutions;
    solutions.best_hope = 100;
    unsigned node_counter = 0, hope;

    while (queue.size() > 0) {
        auto node = queue.back();
        ++node_counter;
        if (is_solved(node->state)) {
            solutions.push_back(node);
            queue.pop_back();
        } else {
            queue.pop_back();
            hope = node->depth + estimate(node->state);
            if (hope <= max_depth) {
                auto children = expand(node);
                for (auto &&child : children) {
                    queue.push_back(child);
                }
            } else {
                solutions.best_hope = std::min(solutions.best_hope, hope);
            }
        }
    }
    if constexpr (verbose) {
        std::cout << "Nodes generated: " << node_counter << std::endl;
    }
    return solutions;
}

template <bool verbose = false, typename NodePtr, typename Expander,
          typename Pruner, typename SolveCheck>
Solutions<NodePtr> depth_first_search(const NodePtr root,
                                      const Expander &expand,
                                      const Pruner &estimate,
                                      const SolveCheck &is_solved,
                                      const unsigned max_depth = 4) {
    // Overload for solving a single starting position
    std::vector<NodePtr> queue({root});
    return depth_first_search<verbose>(queue, expand, estimate, is_solved,
                                       max_depth);
}

template <bool verbose = false, typename NodePtr, typename Expander,
          typename Pruner, typename SolveCheck>
Solutions<NodePtr> IDAstar(std::vector<NodePtr> roots, const Expander &expand,
                           const Pruner &estimate, const SolveCheck &is_solved,
                           const unsigned max_depth = 20,
                           const unsigned slackness = 0) {
    // Main implementation, starting from any number of root nodes
    unsigned search_depth = 100;
    for (auto root : roots) {
        search_depth =
            std::min(estimate(root->state) + root->depth, search_depth);
    }

    Solutions<NodePtr> solutions;
    while (solutions.size() == 0 && search_depth <= max_depth) {
        if constexpr (verbose) {
            std::cout << "Searching at depth " << search_depth << std::endl;
        }
        solutions = depth_first_search<verbose>(roots, expand, estimate,
                                                is_solved, search_depth);
        search_depth = solutions.best_hope;
    }
    if constexpr (verbose) {
        std::cout << "Solutions found" << std::endl;
    }
    if (slackness > 0) {
        // Find suboptimal with up to `slackness` extra moves
        // This implies that optimal solutions have been found
        // hence the job for those solutions is done twice.
        // I don' think this can be avoided since we need to
        // know optimal to introduce slackness
        search_depth = (max_depth < search_depth + slackness - 1)
                           ? max_depth
                           : search_depth + slackness - 1;
        if (verbose)
            std::cout << "Searching at depth " << search_depth << std::endl;
        solutions = depth_first_search<verbose>(roots, expand, estimate,
                                                is_solved, search_depth);
    }
    if constexpr (verbose) {
        if (solutions.size() == 0) {
            std::cout << "IDA*: No solution found" << std::endl;
        }
    }
    return solutions;
}

template <bool verbose = false, typename NodePtr, typename Expander,
          typename Pruner, typename SolveCheck>
Solutions<NodePtr> IDAstar(const NodePtr root, const Expander &expand,
                           const Pruner &estimate, const SolveCheck &is_solved,
                           const unsigned max_depth = 20,
                           const unsigned slackness = 0) {
    // Overload for solving a single starting position
    std::vector<NodePtr> queue{root};
    return IDAstar<verbose>(queue, expand, estimate, is_solved, max_depth,
                            slackness);
}