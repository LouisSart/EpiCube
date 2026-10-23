#include <iostream>       // std::cout << Move
#include <sstream>        // std::isstream
#include "move.hpp"
#include "cubie_cube.hpp"

namespace Cube333 {

std::ostream &operator<<(std::ostream &os, const Corner &c) {
    switch (c) {
        case ULF: os << "ULF"; break;
        case URF: os << "URF"; break;
        case URB: os << "URB"; break;
        case ULB: os << "ULB"; break;
        case DLF: os << "DLF"; break;
        case DRF: os << "DRF"; break;
        case DRB: os << "DRB"; break;
        case DLB: os << "DLB"; break;
        default : os << "-";
    }
    return os;
}

std::ostream &operator<<(std::ostream &os, const Edge &e) {
    switch (e) {
        case UF: os << "UF"; break;
        case UR: os << "UR"; break;
        case UB: os << "UB"; break;
        case UL: os << "UL"; break;
        case LF: os << "LF"; break;
        case RF: os << "RF"; break;
        case RB: os << "RB"; break;
        case LB: os << "LB"; break;
        case DF: os << "DF"; break;
        case DR: os << "DR"; break;
        case DB: os << "DB"; break;
        case DL: os << "DL"; break;
        default : os << "-";
    }
    return os;
}

std::ostream &operator<<(std::ostream &os, const Move &move) {
    static std::string move_str[N_HTM_MOVES]{"U", "U2", "U'", "D", "D2", "D'",
                                    "R", "R2", "R'", "L", "L2", "L'",
                                    "F", "F2", "F'", "B", "B2", "B'"};

    os << move_str[move];
    return os;
}

const std::vector<Move> allowed_next(const Move m) {
    // Utility function that takes in the last move applied to the current node
    // and returns the set of all moves of different faces
    switch (m) {
        case U ... U3:
            return {D, D2, D3, R,  R2, R3, L, L2, L3, F, F2, F3, B, B2, B3};
        case D ... D3:
            return {R, R2, R3, L, L2, L3, F, F2, F3, B, B2, B3};
        case R ... R3:
            return {U, U2, U3, D,  D2, D3, L, L2, L3, F,  F2, F3, B,  B2, B3};
        case L ... L3:
            return {U, U2, U3, D, D2, D3, F, F2, F3, B, B2, B3};
        case F ... F3:
            return {U, U2, U3, D,  D2, D3, R, R2, R3, L,  L2, L3, B,  B2, B3};
        case B ... B3:
            return {U, U2, U3, D, D2, D3, R, R2, R3, L, L2, L3};
        default:
            return {U, U2, U3, D, D2, D3, R, R2, R3, L, L2, L3, F, F2, F3, B, B2, B3};
    }
}

Move HTM::str_to_move(const std::string& s) {
    static std::unordered_map<std::string, Move> str_map { 
        {"U", U}, {"U2", U2}, {"U'", U3}, {"D", D}, {"D2", D2}, {"D'", D3},
        {"R", R}, {"R2", R2}, {"R'", R3}, {"L", L}, {"L2", L2}, {"L'", L3},
        {"F", F}, {"F2", F2}, {"F'", F3}, {"B", B}, {"B2", B2}, {"B'", B3}
    };
    assert(str_map.contains(s));
    return str_map[s];
}

Sequence<Move> make_sequence(const std::string str_in) {
    Sequence<Move> ret;
    std::istringstream f(str_in);
    std::string s;    
    while (std::getline(f, s, ' ')) {
        ret.push_back(HTM::str_to_move(s));
    }
    return ret;
};

static constexpr Permutation<NC> z2_cp {DRF, DLF, DLB, DRB, URF, ULF, ULB, URB};
static constexpr Permutation<NC> SURF_cp {DRF, URF, ULF, DLF, DRB, URB, ULB, DLB};
static constexpr Permutation<NC> y_cp {URF, URB, ULB, ULF, DRF, DRB, DLB, DLF};
static constexpr Permutation<NC> RL_mirror_cp {URF, ULF, ULB, URB, DRF, DLF, DLB, DRB};

static constexpr Permutation<NC> U_cp{URF, URB, ULB, ULF, DLF, DRF, DRB, DLB};
static constexpr Permutation<NC> R_cp = U_cp.get_conjugate(SURF_cp);
static constexpr Permutation<NC> F_cp = R_cp.get_conjugate(SURF_cp);
static constexpr Permutation<NC> D_cp = U_cp.get_conjugate(z2_cp);
static constexpr Permutation<NC> L_cp = D_cp.get_conjugate(SURF_cp);
static constexpr Permutation<NC> B_cp = L_cp.get_conjugate(SURF_cp);

static constexpr Permutation<NC> CPs[N_HTM_MOVES] {
    U_cp, U_cp * U_cp, U_cp * U_cp * U_cp,
    D_cp, D_cp * D_cp, D_cp * D_cp * D_cp,
    R_cp, R_cp * R_cp, R_cp * R_cp * R_cp,
    L_cp, L_cp * L_cp, L_cp * L_cp * L_cp,
    F_cp, F_cp * F_cp, F_cp * F_cp * F_cp,
    B_cp, B_cp * B_cp, B_cp * B_cp * B_cp,
};

static constexpr Permutation<NE> U_ep {UR, UB, UL, UF, LF, RF, RB, LB, DF, DR, DB, DL};
static constexpr Permutation<NE> SURF_ep {RF, UF, LF, DF, DR, UR, UL, DL, RB, UB, LB, DB};
static constexpr Permutation<NE> y_ep {UR, UB, UL, UF, RF, RB, LB, LF, DR, DB, DL, DF};
static constexpr Permutation<NE> z2_ep {DF, DL, DB, DR, RF, LF, LB, RB, UF, UL, UB, UR};
static constexpr Permutation<NE> R_ep = U_ep.get_conjugate(SURF_ep);
static constexpr Permutation<NE> F_ep = R_ep.get_conjugate(SURF_ep);
static constexpr Permutation<NE> D_ep = U_ep.get_conjugate(z2_ep);
static constexpr Permutation<NE> L_ep = D_ep.get_conjugate(SURF_ep);
static constexpr Permutation<NE> B_ep = L_ep.get_conjugate(SURF_ep);

static constexpr Permutation<NE> EPs[N_HTM_MOVES] {
    U_ep, U_ep * U_ep, U_ep * U_ep * U_ep,
    D_ep, D_ep * D_ep, D_ep * D_ep * D_ep,
    R_ep, R_ep * R_ep, R_ep * R_ep * R_ep,
    L_ep, L_ep * L_ep, L_ep * L_ep * L_ep,
    F_ep, F_ep * F_ep, F_ep * F_ep * F_ep,
    B_ep, B_ep * B_ep, B_ep * B_ep * B_ep,
};

CubieCube::CubieCube(){};
CubieCube::CubieCube(const Sequence<Move> &scramble) { apply(scramble); }
CubieCube::CubieCube(const std::string &scramble) { apply(make_sequence(scramble)); }

void CubieCube::corner_apply(const Move& m) {
    cp.compose(CPs[m]);
    ep.compose(EPs[m]);
}

void CubieCube::edge_apply(const Move& m) {};

void CubieCube::apply(const Move& m) {
    corner_apply(m);
    edge_apply(m);
}

void CubieCube::apply(const Sequence<Move> &seq) {
    for (Move m : seq) {
        apply(m);
    }
};

void CubieCube::show() const {
    // Display the 4 arrays defining a cube cubie level

    std::cout << "CubieCube object:" << '\n';
    std::cout << "  CP: ";
    print_array<unsigned, NC, Corner>(cp);
    std::cout << "  CO: ";
    print_array(co);
    std::cout << "  EP: ";
    print_array<unsigned, NE, Edge>(ep);
    std::cout << "  EO: ";
    print_array(eo);
};

bool operator==(const CubieCube &cc1, const CubieCube &cc2) {
    if (cc1.cp != cc2.cp) return false;
    if (cc1.co != cc2.co) return false;
    if (cc1.ep != cc2.ep) return false;
    if (cc1.eo != cc2.eo) return false;
    return true;
}
}; // namespace Cube 333