#pragma once
#include <array>       // move_conj_table
#include <cassert>     // make sure arguments are correct
#include <tuple>       // symmetry components

#include "../lib/permutation.hpp"  // Move type
#include "move.hpp"  // Move type
#include "utils.hpp"  // print

namespace Cube333 {

constexpr unsigned N_SURF = 3;
constexpr unsigned N_Y = 4;
constexpr unsigned N_Z2 = 2;
constexpr unsigned N_LR = 2;
constexpr unsigned N_SYM = 48;
constexpr unsigned N_ELEM_SYM = 4;

unsigned symmetry_index(const unsigned, const unsigned, const unsigned, const unsigned);
std::tuple<unsigned, unsigned, unsigned, unsigned> symmetry_index_to_num(const unsigned);
Move move_conj(const Move &, const unsigned &);
Move move_anti_conj(const Move &, const unsigned &);
Sequence<Move> symmetrize(const Sequence<Move> &, const unsigned &);
Sequence<Move> anti_symmetrize(const Sequence<Move> &, const unsigned &);
unsigned sym_combine(const unsigned &, const unsigned &);

}; // namespace Cube333