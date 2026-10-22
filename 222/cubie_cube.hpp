#pragma once
#include "permutation.hpp"
#include "move.hpp"

namespace Cube222 {

constexpr unsigned NC = 8;
constexpr unsigned NO = 3;

class CubieCube{
    Permutation<NC - 1> cp;
    Orientation<NC - 1, NO> co;

public:
    void apply(const Move & m);
    void index() const;
};

void hello_world();

}; // namespace Cube222