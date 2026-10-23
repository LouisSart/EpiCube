#pragma once

#include <array>
#include <sstream>  // std::string

namespace Cube333 {
constexpr unsigned NC = 8, NE = 12;
enum Corner : unsigned { ULF, URF, URB, ULB, DLF, DRF, DRB, DLB, NONE_C };
enum Edge : unsigned { UF, UR, UB, UL, LF, RF, RB, LB, DF, DR, DB, DL, NONE_E };

std::ostream &operator<<(std::ostream &, const Corner &);
std::ostream &operator<<(std::ostream &, const Edge &);
}; // namespace Cube333