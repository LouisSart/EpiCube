#include "../lib/move.hpp"
#include "../333/cubie_cube.hpp"
#include "../333/defs.hpp"
#include "../333/move.hpp"
#include "../333/symmetry.hpp"

using namespace Cube333;
auto no_effect = make_sequence("R U R' F' U2 L' U' L F U2");
auto Tperm = make_sequence("R U R' U' R' F R2 U' R' U' R U R' F'");
auto corner_3_cycle = make_sequence("R U R' D R U' R' D'");
auto edge_3_cycle = make_sequence("R L' U2 R' L F2");
auto random_moves = make_sequence("R F' U D' U R B' U' R' D' F2 L2");

void test_move_apply() {
    auto cube = CubieCube();

    cube.apply(no_effect);
    assert(cube.is_solved());
    cube.apply(Tperm);
    cube.apply(Tperm);

    assert(cube.is_solved());

    cube = CubieCube(no_effect);
    assert(cube.is_solved());

    cube = CubieCube(edge_3_cycle);
    cube.apply(edge_3_cycle);
    cube.apply(edge_3_cycle);
    assert(cube.is_solved());

    CubieCube cube_check;
    cube.apply(make_sequence("U R F L B D"));
    cube_check.apply(U);
    cube_check.apply(R);
    cube_check.apply(F);
    cube_check.apply(L);
    cube_check.apply(B);
    cube_check.apply(D);

    assert(cube == cube_check);
}

void test_copy_constructor() {
    CubieCube cc = CubieCube::random_state();
    CubieCube cc_copy;

    cc_copy = cc;
    assert(&cc != &cc_copy);
    assert(cc == cc_copy);

    cc_copy.apply(random_moves);
    assert(cc != cc_copy);

    cc.apply(random_moves);
    assert(cc == cc_copy);
}

void test_parity() {
    auto cube = CubieCube();

    cube.apply(corner_3_cycle);
    assert(cube.cp.parity() == 0);

    cube.apply(edge_3_cycle);
    assert(cube.ep.parity() == 0);

    cube.apply(Tperm);
    assert(cube.cp.parity() == 1);
    assert(cube.ep.parity() == 1);

    cube.apply(no_effect);
    assert(cube.cp.parity() == 1);
    assert(cube.ep.parity() == 1);

    cube.apply(random_moves);
    assert(cube.ep.parity() == 1);
    assert(cube.cp.parity() == 1);

    assert(cube.is_solvable());
}

void test_random_state() {
    auto cube = CubieCube::random_state();
    assert(cube.is_solvable());
    assert(!cube.is_solved());  // This could fail once in a while tho...
}

void test_inverse() {
    auto cube = CubieCube::random_state();
    assert((cube * cube.get_inverse()).is_solved());
}

void test_conjugation() {
    auto cube = CubieCube::random_state();
    auto conj = CubieCube::random_state();

    assert(cube == cube.get_conjugate(conj).get_conjugate(conj.get_inverse()));
}

// SYMMETRY
void test_sym_index() {
    for (unsigned c = 0; c < N_SYM; ++c) {
        auto [i, j, k, l] = symmetry_index_to_num(c);
        assert(c == symmetry_index(i, j, k, l));
    }
}

void test_move_translation() {
    assert(S_URF_move_conj[U] == R);
    assert(z2_move_conj[R3] == L3);
    assert(y_move_conj[L3] == B3);
}

void test_move_permutation() {
    auto test_moves = HTM::moves;

    for (unsigned s = 0; s < N_SYM; ++s) {
        auto [c_surf, c_y, c_z2, c_lr] = symmetry_index_to_num(s);

        // Permute one way
        for (unsigned k_lr = 0; k_lr < c_lr; ++k_lr) {
            permute(test_moves, LR_mirror_move_conj);
        }
        for (unsigned k_z2 = 0; k_z2 < c_z2; ++k_z2) {
            permute(test_moves, z2_move_conj);
        }
        for (unsigned k_y = 0; k_y < c_y; ++k_y) {
            permute(test_moves, y_move_conj);
        }
        for (unsigned k_surf = 0; k_surf < c_surf; ++k_surf) {
            permute(test_moves, S_URF_move_conj);
        }

        // Permute backwards
        for (unsigned k_surf = 0; k_surf < (3 - c_surf) % 3; ++k_surf) {
            permute(test_moves, S_URF_move_conj);
        }
        for (unsigned k_y = 0; k_y < (4 - c_y) % 4; ++k_y) {
            permute(test_moves, y_move_conj);
        }
        for (unsigned k_z2 = 0; k_z2 < c_z2; ++k_z2) {
            permute(test_moves, z2_move_conj);
        }
        for (unsigned k_lr = 0; k_lr < c_lr; ++k_lr) {
            permute(test_moves, LR_mirror_move_conj);
        }
        assert(test_moves == HTM::moves);
    }
}

void test_symmetries() {
    // Testing that S^-1 * c * S * m == S^-1 * (c * m') * S
    // with c a random CubieCube for all S, m
    CubieCube random;
    // CubieCube random = CubieCube::random_state();
    CubieCube check = random;
    
    // **************** TEMP ************
    for (unsigned s : {symmetry_index(0, 0, 0, 1),
                    symmetry_index(0, 0, 1, 0),
                    symmetry_index(0, 1, 0, 0),
                    symmetry_index(1, 0, 0, 0)}) {
        for (Move m : HTM::moves) {
            print("***************************");
            print(s, m, move_conj(m, s));
            CubieCube cc1 = random.get_conjugate(s);
            cc1.apply(move_conj(m, s));

            CubieCube cc2 = random;
            cc2.apply(m);
            cc2 = cc2.get_conjugate(s);

            assert(cc1 == cc2);
        }
    }
    // ********************** TEMP *********************

    for (unsigned s = 0; s < N_SYM; ++s) {
        auto [c_surf, c_y, c_z2, c_lr] = symmetry_index_to_num(s);
        print(c_surf, c_y, c_z2, c_lr);
        for (Move m : HTM::moves) {
            print("***************************");
            print(s, m, move_conj(m, s));
            CubieCube cc1 = random.get_conjugate(s);
            cc1.apply(move_conj(m, s));

            CubieCube cc2 = random;
            cc2.apply(m);
            cc2 = cc2.get_conjugate(s);

            assert(cc1 == cc2);
        }
    }
}

void test_combinations() {
    for (unsigned s1 = 0; s1 < 1; ++s1) {
        for (unsigned s2 = 0; s2 < N_SYM; ++s2) {
            for (Move m : HTM::moves) {
                assert(move_conj(move_conj(m, s1), s2) ==
                       move_conj(m, sym_combine(s1, s2)));
            }
        }
    }
}

int main() {
    test_move_apply();
    test_copy_constructor();
    test_parity();
    test_random_state();
    test_inverse();
    test_conjugation();
    test_sym_index();
    test_move_translation();
    test_move_permutation();
    test_symmetries();
    // test_combinations();
    return 0;
}
