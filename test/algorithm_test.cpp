#include "algorithm.hpp"
#include "cubie_cube.hpp"
#include <iostream>

void test_algorithm() {
  auto alg = Algorithm("(R U R' U' R' F R2 U' R' U' R U R' F')");
  assert(alg.size() == 14);
  assert(alg.inv_flag == true);
  assert(alg.back() == F3);

  auto alg2 = Algorithm({R, U, R3, U3, R3, F, R2, U3, R3, U3, R, U, R3, F3});
  assert(alg2.size() == 14);
  assert(alg2.inv_flag == false);
  assert(alg2.back() == F3);

  auto alg3 =
      Algorithm({R, U, R3, U3, R3, F, R2, U3, R3, U3, R, U, R3, F3}, false);
  assert(alg3.size() == 14);
  assert(alg3.inv_flag == false);
  assert(alg3.back() == F3);

  auto step1 = StepAlgorithm("R' U' F", "padding");
  auto step2 = StepAlgorithm("(D L B2)");
  auto step3 = StepAlgorithm("U2 R F'", "finish");
  auto skel = Skeleton({step1, step2, step3});
  assert(skel.size() == 3);
  assert(skel[1].size() == 3);
}

void test_apply() {
  auto cube = CubieCube();

  cube.apply(Algorithm("R U R' F' U2 L' U' L F U2"));
  assert(cube.is_solved());

  cube.apply(Algorithm("R' U' L2 B2 R' D' U2 F R F"));
  cube.apply(StepAlgorithm("(R' U' L2 B2 R' D' U2 F R F)", "inverse"));
  assert(cube.is_solved());

  // Test a random Mallard solve
  cube.apply(Algorithm("R' U' F R2 D' L F' D2 R2 D2 L2 F' U2 B2 U' B' L' U' B2 "
                       "U' R' D' R' U' F"));
  auto skel =
      Skeleton({StepAlgorithm("U B"), StepAlgorithm("(D' B')"),
                StepAlgorithm("(L' U')"), StepAlgorithm("L2 D R' D2 R"),
                StepAlgorithm("L2 U F2 R2 D"), StepAlgorithm("F2 U2 L2")});
  cube.apply(skel);
  assert(cube.is_solved());
}

int main() {
  test_algorithm();
  test_apply();
  return 0;
}