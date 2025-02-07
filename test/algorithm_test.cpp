#include "algorithm.hpp"
#include <iostream>

void test_algorithm() {
  auto alg = Algorithm("(R U R' U' R' F R2 U' R' U' R U R' F')");
  assert(alg.size() == 14);
  assert(alg.inv_flag == true);
  assert(alg.back() == F3);

  auto step1 = StepAlgorithm("R' U' F", "padding");
  auto step2 = StepAlgorithm("(D L B2)");
  auto step3 = StepAlgorithm("U2 R F'", "finish");
  auto skel = Skeleton({step1, step2, step3});
  assert(skel.size() == 3);
  assert(skel[1].size() == 3);
}

int main() {
  test_algorithm();
  return 0;
}