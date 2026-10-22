#include "../lib/search.hpp"
#include "../lib/utils.hpp"
#include "../lib/pruning_table.hpp"
#include "../222/cubie_cube.hpp"
#include "../222/move.hpp"
#include <set>
#include <vector>
#include <cassert>

using namespace Cube222; 
unsigned index(const CubieCube &cube){return cube.index();};
void from_index(const unsigned& c, CubieCube &cube){cube.from_index(c);}

int main(int argc, const char* argv[]) {
    CubieCube cc;
    cc.apply({U, U2, U3, U2});
    assert(cc.is_solved());
    cc.apply({R, R2, R3, R2});
    assert(cc.is_solved());
    cc.apply({F, F2, F3, F2});
    assert(cc.is_solved());
    cc.apply({R, U, R3, F3, U2, R3, F3, R, F, U2});
    assert(cc.is_solved());

    PruningTable<CARD> ptable;
    ptable.generate(index, from_index, moves, 6, 9);
    // ptable.generate(index, from_index, std::array{U, U3, R, R3, F, F3}, 6, 9); // QTM
    ptable.show_distribution();

    // HTM :
    //  0 1
    //  1 9
    //  2 54
    //  3 321
    //  4 1847
    //  5 9992
    //  6 50136
    //  7 227536
    //  8 870072
    //  9 1887748
    // 10 623800
    // 11 2644
    // Mean value: 8.75558

    // QTM :
    //  0 1
    //  1 6
    //  2 27
    //  3 120
    //  4 534
    //  5 2256
    //  6 8969
    //  7 33058
    //  8 114149
    //  9 360508
    // 10 930588
    // 11 1350852
    // 12 782536
    // 13 90280
    // 14 276
    // Mean value: 10.6664
}