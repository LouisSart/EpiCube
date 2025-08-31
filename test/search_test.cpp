#include "search.hpp"

#include "algorithm.hpp"
#include "cubie_cube.hpp"
#include "node.hpp"

auto expand(const Node<CubieCube>::sptr node) {
    std::vector<Node<CubieCube>::sptr> ret;
    for (const Move &move : standard_directions(node)) {
        CubieCube cc = node->state;
        cc.apply(move);
        ret.emplace_back(
            new Node(cc, node->depth + 1, node, node->inverse, move));
    }
    return ret;
}

bool is_solved(const CubieCube &cube) { return cube.is_solved(); }
unsigned estimate(const CubieCube &cube) {
    if (cube.is_solved())
        return 0;
    else
        return 1;
}

void test_overloads() {
    auto state = CubieCube();
    state.apply(Algorithm("R' U' F"));
    auto root = make_root(state);
    auto root_inverse = make_root(state.get_inverse(), true);

    auto solutions =
        IDAstar(std::vector{root, root_inverse}, expand, estimate, is_solved);
    solutions.show();

    solutions = IDAstar(root, expand, estimate, is_solved);
}

int main() { test_overloads(); }
