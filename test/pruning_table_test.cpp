#include "pruning_table.hpp"

#include <cassert>
#include <iostream>

#include "coordinate.hpp"
#include "cubie_cube.hpp"

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
    auto index = [](const CubieCube &cc) { return permutation_index(cc.cp); };
    auto from_index = [](const unsigned &coord) {
        CubieCube ret;
        permutation_from_index(coord, ret.cp);
        return ret;
    };

    pt.generate<true>(CubieCube(), apply, index, from_index);
    assert(pt.is_filled());

    pt.write("pruning_tables/all_corners.dat");
    PruningTable<N> reload;
    bool check = reload.load("pruning_tables/all_corners.dat");

    assert(check);

    assert(reload.is_filled());
    for (unsigned i = 0; i < N; ++i) {
        assert(reload.estimate(i) == pt.estimate(i));
    }

    // pt.show_distribution();
}

int main() {
    test_api();
    test_generate();
    return 0;
}