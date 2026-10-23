#include "../333/cubie_cube.hpp"
#include "../lib/move.hpp"
#include "../333/move.hpp"

using namespace Cube333;

void move_test() {
    print_array(HTM::moves);
    Sequence<Move> seq {R, U, R3, U3, R3, F, R2, U3, R3, U3, R, U, R3, F3};
    seq.show();
}

auto no_effect = make_sequence("R U R' F' U2 L' U' L F U2");
auto Tperm = make_sequence("R U R' U' R' F R2 U' R' U' R U R' F'");
auto corner_3_cycle = make_sequence("R U R' D R U' R' D'");
auto edge_3_cycle = make_sequence("R L' U2 R' L F2");
auto random_moves = make_sequence("R F' U D' U R B' U' R' D' F2 L2");

void test_move_apply() {
    edge_3_cycle.show();
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

int main(){
    move_test();
    test_move_apply();
    return 0;
}