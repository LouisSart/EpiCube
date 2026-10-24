#include "../lib/move.hpp"
#include "../333/cubie_cube.hpp"
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


int main() {
    test_move_apply();
    test_copy_constructor();
    test_parity();
    test_random_state();
    test_inverse();
    test_conjugation();
    return 0;
}
