#include "../lib/utils.hpp"
#include "../222/cubie_cube.hpp"
#include "../222/move.hpp"
#include <cassert>

int main(int argc, const char* argv[]) {
    using namespace Cube222;
    CubieCube cc;
    cc.apply({U, U2, U3, U2});
    assert(cc.is_solved());
    cc.apply({R, R2, R3, R2});
    assert(cc.is_solved());
    cc.apply({F, F2, F3, F2});
    assert(cc.is_solved());
    cc.apply({R, U, R3, F3, U2, R3, F3, R, F, U2});
    assert(cc.is_solved());
}