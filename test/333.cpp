#include "../lib/move.hpp"
#include "../lib/move_table.hpp"
#include "../lib/pruning_table.hpp"
#include "../lib/search.hpp"
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
    CubieCube random = CubieCube::random_state();

    for (unsigned s = 0; s < N_SYM; ++s) {
        auto [c_surf, c_y, c_z2, c_lr] = symmetry_index_to_num(s);
        for (Move m : HTM::moves) {
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


void test_algorithm() {
    auto alg = make_sequence("(R U R' U' R' F R2 U' R' U' R U R' F')");
    assert(alg.size() == 14);
    assert(alg.inv_flag == true);
    assert(alg.back() == F3);

    auto alg2 = Sequence<Move>({R, U, R3, U3, R3, F, R2, U3, R3, U3, R, U, R3, F3});
    assert(alg2.size() == 14);
    assert(alg2.inv_flag == false);
    assert(alg2.back() == F3);
}

void test_apply() {
    auto cube = CubieCube();

    cube.apply(make_sequence("R U R' F' U2 L' U' L F U2"));
    assert(cube.is_solved());

    cube.apply(make_sequence("R' U' L2 B2 R' D' U2 F R F"));
    cube.apply(make_sequence("(R' U' L2 B2 R' D' U2 F R F)"));
    assert(cube.is_solved());
}

void test_move_table() {
    constexpr unsigned N = factorial(NC);
    MoveTable<N, N_HTM_MOVES> move_table;

    auto index = [](const CubieCube &cc) { return cc.cp.index(); };
    auto from_index = [](const unsigned &c, CubieCube& cc) {
        cc.cp.set_from_index(c);
    };

    move_table.compute<CubieCube>(index, from_index, HTM::moves);
    move_table.write("obj/mtable");
    assert(move_table.is_filled());

    CubieCube cc;
    unsigned c = 0;
    for (Move m : HTM::moves) {
        cc.apply(m);
        move_table.apply(m, c);
        assert(c == index(cc));
    }

    MoveTable<N, N_HTM_MOVES> loaded_move_table;
    assert(loaded_move_table.load("obj/mtable"));

    // Check that the loaded table is the same as the original
    for (unsigned c = 0; c < N; ++c) {
        for (Move m : HTM::moves) {
            assert(move_table.table[c * N_HTM_MOVES + m] ==
                   loaded_move_table.table[c * N_HTM_MOVES + m]);
        }
    }
}

void test_api() {
    PruningTable<10> pt;

    assert(pt.size() == 10);

    // Test initial state
    for (unsigned i = 0; i < pt.size(); ++i) {
        assert(pt.estimate(i) == PruningTable<10>::unassigned);
    }

    // Set some values
    pt.set(0, 5);
    pt.set(1, 3);
    assert(pt.estimate(0) == 5);
    assert(pt.estimate(1) == 3);

    // Check is_assigned
    assert(pt.is_assigned(0));

    pt.reset();
    assert(pt.estimate(0) == PruningTable<10>::unassigned);
    assert(pt.estimate(1) == PruningTable<10>::unassigned);

    auto estimator = pt.get_estimator();
    pt.set(1, 3);
    assert(estimator(0) == PruningTable<10>::unassigned);
    assert(estimator(1) == 3);
}

void test_generate() {
    // Pruning table to solve cp
    constexpr auto N = factorial(8);
    PruningTable<N> pt;

    auto apply = [](const Move &move, CubieCube &cc) { cc.apply(move); };
    auto index = [](const CubieCube &cc) { return cc.cp.index(); };
    auto from_index = [](const unsigned &coord, CubieCube&cc) {
        cc.cp.set_from_index(coord);
    };

    pt.generate<CubieCube, false>(index, from_index, HTM::moves, 4, 6);
    assert(pt.is_filled());

    fs::path tmp {"tmp/"};
    pt.write(tmp / "corners.dat");
    PruningTable<N> reload;
    bool check = reload.load(tmp / "corners.dat");

    assert(check);

    assert(reload.is_filled());
    for (unsigned i = 0; i < N; ++i) {
        assert(reload.estimate(i) == pt.estimate(i));
    }

    fs::remove_all(tmp);
}


bool is_solved(const CubieCube &cube) { return cube.is_solved(); }
unsigned estimate(const CubieCube &cube) {
    if (cube.is_solved())
        return 0;
    else
        return 1;
}

std::vector<Move> custom_directions(const Node<CubieCube>::sptr node) {
    return {U, U2, U3, R, R2, R2, F, F2, F3};
}

void test_overloads() {
    auto state = CubieCube();
    state.apply(make_sequence("R' U' F"));
    auto root = make_root(state);
    auto root_inverse = make_root(state.get_inverse(), true);

    // Multiple starting positions
    auto solutions =
        IDAstar<false, CubieCube>({root, root_inverse}, estimate, is_solved, custom_directions);

    for (auto sol : solutions) {
        sol->get_path<Move>();
    }

    // Single starting position
    solutions = IDAstar<false, CubieCube>(root, estimate, is_solved, custom_directions);
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
    test_combinations();
    test_algorithm();
    test_apply();
    test_move_table();
    test_api();
    test_generate();
    test_overloads();
    return 0;
}
