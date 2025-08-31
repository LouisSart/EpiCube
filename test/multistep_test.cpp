#include "multistep.hpp"

#include "node.hpp"
#include "search.hpp"

void apply(const Move& move, CubieCube& cube) { cube.apply(move); }
bool step_one_is_solved(const CubieCube& cube) {
    return cube.ep[UF] == UF && cube.eo[UF] == 0;
}

bool step_one_estimate(const CubieCube& cube) {
    if (step_one_is_solved(cube)) return 0;
    return 1;
}

auto step_one(std::vector<StepNode>& roots, unsigned slackness,
              unsigned max_depth) {
    std::vector<Node<CubieCube>::sptr> queue;
    for (auto root : roots) {
        queue.push_back(make_root(root.state));
    }
    auto solutions = IDAstar(queue, apply, step_one_estimate,
                             step_one_is_solved, max_depth, slackness);
    solutions.show();
}

int main() {
    CubieCube cube;
    cube.apply(
        "R' U' F B2 U2 R2 F2 D2 L2 U2 R2 D B2 L' D F2 U' L2 F D2 L' U' R' U' "
        "F");
    StepNode root(cube);
    std::vector<StepNode> roots{root};
    step_one(roots, 1, 20);
    return 0;
}