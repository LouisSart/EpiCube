#pragma once
#include <array>       // move_conj_table
#include <cassert>     // make sure arguments are correct
#include <tuple>       // symmetry components

#include "../lib/permutation.hpp"  // Move type
#include "move.hpp"  // Move type
#include "utils.hpp"  // print

namespace Cube333 {

Move move_conj(const Move &m, const unsigned &s);
Move move_anti_conj(const Move &m, const unsigned &s);
Sequence<Move> symmetrize(const Sequence<Move> &alg, const unsigned &sym_index);
Sequence<Move> anti_symmetrize(const Sequence<Move> &alg, const unsigned &sym_index);
unsigned sym_combine(const unsigned &, const unsigned &);

}; // namespace Cube333