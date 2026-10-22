#pragma once
#include "permutation.hpp"
#include "move.hpp"
#include "../lib/move.hpp"

namespace Cube222 {

constexpr unsigned NC = 8;
constexpr unsigned NO = 3;

struct CubieCube{
    Permutation<NC - 1> cp;
    Orientation<NC - 1, NO> co;

    void apply(const Move &);
    void apply(const Sequence<Move> &);
    unsigned index() const;
    void show() const;
    bool is_solved() const;
};

}; // namespace Cube222