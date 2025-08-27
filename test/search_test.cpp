#include "search.hpp"

#include "algorithm.hpp"
#include "cubie_cube.hpp"
#include "node.hpp"
#include "step_node.hpp"

void apply(const Move &move, CubieCube &cube) { cube.apply(move); }
bool is_solved(const CubieCube &cube) { return cube.is_solved(); }
unsigned estimate(const CubieCube &cube) {
    if (cube.is_solved())
        return 0;
    else
        return 1;
}

std::vector<Move> custom_directions(const Node<CubieCube>::sptr node) {
    return {U, U2, U3, R, R2, R2, F, F2, F3};
}

void test_overloads() {
    auto state = CubieCube();
    state.apply(Algorithm("R' U' F"));
    auto root = make_root(state);
    auto root_inverse = make_root(state.get_inverse());

    auto solutions =
        IDAstar(std::deque{root, root_inverse}, apply, estimate, is_solved);

    for (auto sol : solutions) {
        sol->get_path(sol->get_root() == root_inverse).show();
    }

    solutions = IDAstar(root, apply, estimate, is_solved);
    solutions = IDAstar(root, apply, estimate, is_solved, custom_directions);
    solutions = IDAstar(std::deque{root, root_inverse}, apply, estimate,
                        is_solved, custom_directions);
}

int main() { test_overloads(); }
