#pragma once
#include <array>          // arrays of move strings
#include <iostream>       // std::cout << Move
#include <sstream>        // std::string
#include <unordered_map>  // map move to str
#include <vector>         // vectors of moves
#include <cassert>

namespace Cube333 {

    constexpr unsigned N_HTM_MOVES = 18;
enum Move { U, U2, U3, D, D2, D3, R, R2, R3, L, L2, L3, F, F2, F3, B, B2, B3 };
std::ostream &operator<<(std::ostream &, const Move &);

const std::vector<Move> allowed_next(const Move);

struct HTM {
    static Move str_to_move(const std::string& s) {
        static std::unordered_map<std::string, Move> str_map { 
            {"U", U}, {"U2", U2}, {"U'", U3}, {"D", D}, {"D2", D2}, {"D'", D3},
            {"R", R}, {"R2", R2}, {"R'", R3}, {"L", L}, {"L2", L2}, {"L'", L3},
            {"F", F}, {"F2", F2}, {"F'", F3}, {"B", B}, {"B2", B2}, {"B'", B3}
        };
        assert(str_map.contains(s));
        return str_map[s];
    }

    static constexpr std::array<Move, N_HTM_MOVES> moves{
        U, U2, U3, D, D2, D3, R, R2, R3, L, L2, L3, F, F2, F3, B, B2, B3};

    static constexpr Move inverse[N_HTM_MOVES]{
        [U] = U3, [U2] = U2, [U3] = U, [D] = D3, [D2] = D2, [D3] = D,
        [R] = R3, [R2] = R2, [R3] = R, [L] = L3, [L2] = L2, [L3] = L,
        [F] = F3, [F2] = F2, [F3] = F, [B] = B3, [B2] = B2, [B3] = B};
};
// constexpr unsigned N_ROTATIONS = 9;
// std::ostream &operator<<(std::ostream &os, const Rotation &rot) {
//     static std::string rot_str[N_ROTATIONS]{"x",  "x2", "x'", "y", "y2",
//                                     "y'", "z",  "z2", "z'"};
//     os << rot_str[rot];
//     return os;
// }
// enum Rotation { x, x2, x3, y, y2, y3, z, z2, z3 };

// Rotation inverse_of_rotations[N_ROTATIONS]{
//     [x] = x3, [x2] = x2, [x3] = x,  [y] = y3, [y2] = y2,
//     [y3] = y, [z] = z3,  [z2] = z2, [z3] = z};
};