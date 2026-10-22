#include "utils.hpp"
#include "cubie_cube.hpp"
#include "move.hpp"

namespace Cube222 {
std::ostream &operator<<(std::ostream &os, const Move &m) {
    switch (m) {
        case U:  os << "U";  break;
        case U2: os << "U2"; break;
        case U3: os << "U'"; break;
        case R:  os << "R";  break;
        case R2: os << "R2"; break;
        case R3: os << "R'"; break;
        case F:  os << "F";  break;
        case F2: os << "F2"; break;
        case F3: os << "F'"; break;
        default: os << "";
    }
    return os;
}


static Permutation<NC - 1> U_cp {3, 0, 1, 2, 4, 5, 6}; // U move corner permutation
static Permutation<NC - 1> SURF_cp {5, 4, 0, 3, 6, 2, 1}; // Rotation around the URF corner
static Permutation<NC - 1> SURF2_cp = SURF_cp.get_inverse();
static Permutation<NC - 1> R_cp = U_cp.get_conjugate(SURF_cp); // R move
static Permutation<NC - 1> F_cp = R_cp.get_conjugate(SURF_cp); // F move
static Permutation<NC - 1> M_cp {2, 1, 0, 3, 6, 5, 4}; // Planar mirror

static Permutation<NC - 1> CPs[N_MOVE] {
    U_cp,                   // U 
    U_cp * U_cp,            // U2
    U_cp.get_inverse(),     // U'
    R_cp,                   // R
    R_cp * R_cp,            // R2
    R_cp * R_cp * R_cp,     // R'
    F_cp,                   // F
    F_cp * F_cp,            // F2
    F_cp * F_cp * F_cp,     // F' move
};

static Orientation<NC - 1, NO> SURF_co {2, 1, 2, 1, 1, 2, 1};
static Orientation<NC - 1, NO> SURF2_co = (SURF_co * SURF_cp) + SURF_co;
static Orientation<NC - 1, NO> R_co = (SURF2_co * U_cp * SURF_cp) + SURF_co;
static Orientation<NC - 1, NO> F_co = (SURF_co * U_cp * SURF2_cp) + SURF2_co;
static Orientation<NC - 1, NO> COs[N_MOVE] {
    {0, 0, 0, 0, 0, 0, 0},                  // U
    {0, 0, 0, 0, 0, 0, 0},                  // U2
    {0, 0, 0, 0, 0, 0, 0},                  // U'
    R_co,                                   // R
    (R_co * R_cp) + R_co,                   // R2
    ((R_co * R_cp) + R_co) * R_cp + R_co,   // R'
    F_co,                                   // F
    (F_co * F_cp) + F_co,                   // F2
    ((F_co * F_cp) + F_co) * F_cp + F_co,   // F'
};

void CubieCube::apply(const Move & m) {
    cp.compose(CPs[m]);
    co = (co * CPs[m]) + COs[m];
}

void CubieCube::apply(const Sequence<Move> &seq) {
    for (Move m : seq) {
        apply(m);
    }
}

unsigned CubieCube::index() const {
    return cp.index();
}

void CubieCube::show() const {
    print_array(cp);
    print_array(co);
}

bool CubieCube::is_solved() const {
    return cp.is_solved() && co.is_solved();
};
};
