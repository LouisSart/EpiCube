#pragma once
#include <array>          // arrays of move strings
#include <unordered_map>  // map move to str
#include <vector>         // vectors of moves
#include <cassert>
#include "../lib/move.hpp"

namespace Cube333 {

constexpr unsigned N_HTM_MOVES = 18;
enum Move { U, U2, U3, D, D2, D3, R, R2, R3, L, L2, L3, F, F2, F3, B, B2, B3 };
std::ostream &operator<<(std::ostream &, const Move &);

const std::vector<Move> allowed_next(const Move);

struct HTM {
    static Move str_to_move(const std::string&);

    static constexpr std::array<Move, N_HTM_MOVES> moves{
        U, U2, U3, D, D2, D3, R, R2, R3, L, L2, L3, F, F2, F3, B, B2, B3};

    static constexpr Move inverse[N_HTM_MOVES]{
        [U] = U3, [U2] = U2, [U3] = U, [D] = D3, [D2] = D2, [D3] = D,
        [R] = R3, [R2] = R2, [R3] = R, [L] = L3, [L2] = L2, [L3] = L,
        [F] = F3, [F2] = F2, [F3] = F, [B] = B3, [B2] = B2, [B3] = B};
};

Sequence<Move> make_sequence(const std::string);
};