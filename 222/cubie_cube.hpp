#pragma once
#include "permutation.hpp"
#include "move.hpp"
#include "../lib/move.hpp"

namespace Cube222 {

constexpr unsigned NC = 8;
constexpr unsigned NO = 3;
constexpr unsigned NCP = Permutation<NC - 1>::CARD;
constexpr unsigned NCO = Orientation<NC - 1, NO>::CARD / NO;
constexpr unsigned CARD = NCP * NCO;

struct CubieCube{
    Permutation<NC - 1> cp;
    Orientation<NC - 1, NO> co;

    void apply(const Move &);
    void apply(const Sequence<Move> &);
    unsigned index() const;
    void from_index(const unsigned &);
    void show() const;
    bool is_solved() const;
};

}; // namespace Cube222