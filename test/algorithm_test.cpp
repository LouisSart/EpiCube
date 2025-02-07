#include "algorithm.hpp"
#include <iostream>

void test_algorithm() {
  auto alg = Algorithm("(R U R' U' R' F R2 U' R' U' R U R' F')");
  std::cout << alg << std::endl;
}

int main() {
  test_algorithm();
  return 0;
}