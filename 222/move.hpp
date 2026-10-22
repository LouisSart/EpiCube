#pragma once
#include <iostream>

namespace Cube222 {

constexpr unsigned N_MOVE = 6;
constexpr unsigned N_SYM = 9;

enum Move {U, U2, U3, R, R2, R3, F, F2, F3};
std::ostream &operator<<(std::ostream &os, const Move &m);

}; // namespace Cube222
