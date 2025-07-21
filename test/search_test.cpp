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

int main() {
  auto state = CubieCube();
  state.apply(Algorithm("R' U' F"));
  auto root = make_root(state);

  auto solutions = IDAstar(root, apply, estimate, is_solved);
  solutions.show();
}