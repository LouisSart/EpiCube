#include "coordinate.hpp"

#include <cassert>
#include <iostream>

#include "cubie.hpp"
#include "move_table.hpp"
#include "utils.hpp"

void test_layout_index() {
    std::array<unsigned, NC> clayout;
    for (unsigned np = 0; np <= NC; ++np) {
        for (unsigned cl = 0; cl < binomial(NC, np); ++cl) {
            layout_from_index(cl, clayout, np);
            assert(layout_index(clayout, np) == cl);
        }
    }

    std::array<unsigned, NE> elayout;
    for (unsigned np = 0; np <= NE; ++np) {
        for (unsigned cl = 0; cl < binomial(NE, np); ++cl) {
            layout_from_index(cl, elayout, np);
            assert(layout_index(elayout, np) == cl);
        }
    }
}

void test_permutation_index() {
    std::array<unsigned, NC> cperm;

    for (unsigned c = 0; c < factorial(NC); ++c) {
        permutation_from_index(c, cperm);
        assert(c == permutation_index(cperm));
    }
}

void test_eo_index() {
    std::array<unsigned, NE> eo;

    for (unsigned c = 0; c < ipow(2, NE); ++c) {
        eo_from_index(c, eo);
        assert(c == eo_index(eo));
    }
}

void test_co_index() {
    std::array<unsigned, NC> co;

    for (unsigned c = 0; c < ipow(3, NC); ++c) {
        co_from_index(c, co);
        assert(c == co_index(co));
    }
}

void test_move_table() {
    constexpr unsigned N = factorial(NC);
    MoveTable<N> move_table;

    auto index = [](const CubieCube &cc) { return permutation_index(cc.cp); };
    auto from_index = [](const unsigned &c) {
        CubieCube ret;
        permutation_from_index(c, ret.cp);
        return ret;
    };

    move_table.compute(index, from_index);
    move_table.write("corners");
    assert(move_table.is_filled());

    CubieCube cc;
    unsigned c;
    for (Move m : {R, U3, F2, B, D3, L2}) {
        cc.apply(m);
        move_table.apply(c, m);
        assert(c == index(cc));
    }

    MoveTable<N> loaded_move_table;
    assert(loaded_move_table.load("corners"));

    // Check that the loaded table is the same as the original
    for (unsigned c = 0; c < N; ++c) {
        for (Move m : HTM_Moves) {
            assert(move_table.table[c * N_HTM_MOVES + m] ==
                   loaded_move_table.table[c * N_HTM_MOVES + m]);
        }
    }
}

int main() {
    test_layout_index();
    test_permutation_index();
    test_eo_index();
    test_co_index();
    test_move_table();
}