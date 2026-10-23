#include "../333/cubie_cube.hpp"
#include "../lib/move.hpp"
#include "../333/move.hpp"

using namespace Cube333;

void move_test() {
    print_array(HTM::moves);
    Sequence<Move> seq {R, U, R3, U3, R3, F, R2, U3, R3, U3, R, U, R3, F3};
    seq.show();
}

int main(){
    move_test();
    CubieCube cc, check;
    cc.show();
    assert(cc == check);
    return 0;
}