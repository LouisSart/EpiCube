#include "multistep.hpp"

#include "node.hpp"
#include "search.hpp"

// General functions
void apply(const Move& move, CubieCube& cube) { cube.apply(move); }
auto expand = make_expander<CubieCube>(
    [](const Move& move, CubieCube& cc) { cc.apply(move); },
    standard_directions<Node<CubieCube>::sptr>);

auto make_stepper(auto& expand, auto& estimate, auto& is_solved) {
    return [&expand, &estimate, &is_solved](
               std::vector<typename StepNode::sptr>& step_roots,
               unsigned slackness) {
        std::vector<typename StepNode::sptr> ret;
        CubieCube state;
        for (auto step_root : step_roots) {
            auto root = make_root(step_root->state);
            auto root_inv = make_root(step_root->state.get_inverse(), true);
            auto roots = std::vector({root, root_inv});
            auto solutions =
                IDAstar(roots, expand, estimate, is_solved, 20, slackness);
            for (auto sol : solutions) {
                if (sol->inverse)
                    state = sol->state.get_inverse();
                else
                    state = sol->state;
                ret.emplace_back(new StepNode(state, sol->get_path(), step_root,
                                              step_root->depth + sol->depth));
            }
        }
        return ret;
    };
}

// Step one = solve the UF edge
bool step_one_is_solved(const CubieCube& cube) {
    return cube.ep[UF] == UF && cube.eo[UF] == 0;
}
bool step_one_estimate(const CubieCube& cube) {
    if (step_one_is_solved(cube)) return 0;
    return 1;
}

auto step_one = make_stepper(expand, step_one_estimate, step_one_is_solved);

// Step 2 = solve the UR Edge
bool step_two_is_solved(const CubieCube& cube) {
    return step_one_is_solved(cube) && cube.ep[UR] == UR && cube.eo[UR] == 0;
}

bool step_two_estimate(const CubieCube& cube) {
    if (step_two_is_solved(cube)) return 0;
    return 1;
}

auto step_two = make_stepper(expand, step_two_estimate, step_two_is_solved);

// Step 3 = solve the UB Edge
bool step_three_is_solved(const CubieCube& cube) {
    return step_two_is_solved(cube) && cube.ep[UB] == UB && cube.eo[UB] == 0;
}
bool step_three_estimate(const CubieCube& cube) {
    if (step_three_is_solved(cube)) return 0;
    return 1;
}

auto step_three =
    make_stepper(expand, step_three_estimate, step_three_is_solved);

// Step 4 = solve the UL Edge
bool step_four_is_solved(const CubieCube& cube) {
    return step_three_is_solved(cube) && cube.ep[UL] == UL && cube.eo[UL] == 0;
}

bool step_four_estimate(const CubieCube& cube) {
    if (step_four_is_solved(cube)) return 0;
    return 1;
}
auto step_four = make_stepper(expand, step_four_estimate, step_four_is_solved);

int main() {
    CubieCube cube;
    cube.apply(
        "R' U' F B2 U2 R2 F2 D2 L2 U2 R2 D B2 L' D F2 U' L2 F D2 L' U' R' U' "
        "F");
    auto root = make_step_root(cube);
    std::vector<StepNode::sptr> roots{root};
    auto solutions =
        jaap_multistep(roots, 2, step_one, step_two, step_three, step_four);
    assert(solutions.size());
    for (auto sol : solutions) {
        sol->get_skeleton({"UF", "UR", "UB", "UL"}).show();
        print("");
    }
    // s=0 (debug, no max depth):
    // real    0m0.178s
    // user    0m0.173s
    // sys     0m0.005s

    // s=1 (debug, no max depth)
    // real    0m15.192s
    // user    0m15.200s
    // sys     0m0.004s

    // s=2 (debug, no max depth)
    // real    11m26.994s
    // user    11m26.980s
    // sys     0m0.056

    return 0;
}