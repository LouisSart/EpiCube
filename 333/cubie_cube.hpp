#pragma once
#include "../lib/utils.hpp"
#include "../lib/permutation.hpp"
#include "cubie.hpp"
#include "move.hpp"

namespace Cube333{
struct CubieCube {
    Permutation<NC> cp;
    Orientation<NC, 3> co;
    Permutation<NE> ep;
    Orientation<NE, 2> eo;

    CubieCube();
    CubieCube(
        const Permutation<NC> &, const Orientation<NC, 3> &,
        const Permutation<NE> &, const Orientation<NE, 2> &);
    CubieCube(const Sequence<Move> &);
    CubieCube(const std::string &);
    
    void corner_apply(const Move&);
    void edge_apply(const Move&);
    void apply(const Move&);
    void show() const;
    void apply(const Sequence<Move> &);
    CubieCube get_conjugate(const CubieCube&) const;
    CubieCube get_conjugate(const unsigned&) const;
    CubieCube get_inverse() const;
    bool is_solvable() const;
    bool is_solved() const;
    static CubieCube random_state();
};

bool operator==(const CubieCube &, const CubieCube &);
CubieCube operator*(const CubieCube &, const CubieCube &);
CubieCube operator^(const CubieCube &, const CubieCube &);
}; // namespace Cube333