#include "../lib/utils.hpp"
#include "../222/cubie_cube.hpp"
#include "../222/move.hpp"

int main(int argc, const char* argv[]) {
    using namespace Cube222;
    CubieCube cc;
    cc.index();
    cc.apply(U);
    print(U);
}