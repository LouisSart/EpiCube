#include "multistep.hpp"

#include "node.hpp"
#include "search.hpp"

// General functions
void apply(const Move& move, CubieCube& cube) { cube.apply(move); }
auto expand = make_expander<CubieCube>(
    [](const Move& move, CubieCube& cc) { cc.apply(move); },
    standard_directions<Node<CubieCube>::sptr>);

// Step one = solve the UF edge
bool step_one_is_solved(const CubieCube& cube) {
    return cube.ep[UF] == UF && cube.eo[UF] == 0;
}
bool step_one_estimate(const CubieCube& cube) {
    if (step_one_is_solved(cube)) return 0;
    return 1;
}

auto step_one =
    make_stepper<CubieCube>(expand, step_one_estimate, step_one_is_solved);

// Step 2 = solve the UR Edge
bool step_two_is_solved(const CubieCube& cube) {
    return step_one_is_solved(cube) && cube.ep[UR] == UR && cube.eo[UR] == 0;
}

bool step_two_estimate(const CubieCube& cube) {
    if (step_two_is_solved(cube)) return 0;
    return 1;
}

auto step_two =
    make_stepper<CubieCube>(expand, step_two_estimate, step_two_is_solved);

// Step 3 = solve the UB Edge
bool step_three_is_solved(const CubieCube& cube) {
    return step_two_is_solved(cube) && cube.ep[UB] == UB && cube.eo[UB] == 0;
}
bool step_three_estimate(const CubieCube& cube) {
    if (step_three_is_solved(cube)) return 0;
    return 1;
}

auto step_three =
    make_stepper<CubieCube>(expand, step_three_estimate, step_three_is_solved);

// Step 4 = solve the UL Edge
bool step_four_is_solved(const CubieCube& cube) {
    return step_three_is_solved(cube) && cube.ep[UL] == UL && cube.eo[UL] == 0;
}

bool step_four_estimate(const CubieCube& cube) {
    if (step_four_is_solved(cube)) return 0;
    return 1;
}
auto step_four = make_stepper<CubieCube, false>(expand, step_four_estimate,
                                                step_four_is_solved);

int main() {
    CubieCube cube;
    cube.apply(
        "R' U' F B2 U2 R2 F2 D2 L2 U2 R2 D B2 L' D F2 U' L2 F D2 L' U' R' U' "
        "F");
    auto root = make_step_root(cube);
    std::vector<StepNode::sptr> roots{root};
    auto solutions =
        jaap_solver(roots, 2, step_one, step_two, step_three, step_four);
    assert(solutions.size());
    for (auto sol : solutions) {
        sol->get_skeleton({"UF", "UR", "UB", "UL"}).show();
        print("");
    }
    return 0;
}