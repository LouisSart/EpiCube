#include <iostream>       // std::cout << Move
#include <sstream>        // std::isstream
#include "move.hpp"
#include "cubie_cube.hpp"
#include "defs.hpp"

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

CubieCube::CubieCube(){};
CubieCube::CubieCube(const Sequence<Move> &scramble) { apply(scramble); }
CubieCube::CubieCube(const std::string &scramble) { apply(make_sequence(scramble)); }

void CubieCube::corner_apply(const Move& m) {
    cp.compose(CPs[m]);
    co = (co * CPs[m]) + COs[m];
}

void CubieCube::edge_apply(const Move& m) {
    ep.compose(EPs[m]);
    eo = (eo * EPs[m]) + EOs[m];
};

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

bool CubieCube::is_solved() const {
    if (!cp.is_solved()) return false;
    if (!co.is_solved()) return false;
    if (!ep.is_solved()) return false;
    if (!eo.is_solved()) return false;
    return true;
}

CubieCube CubieCube::random_state() {
    CubieCube cube;
    cube.cp.random_state();
    cube.co.random_state();
    cube.ep.random_state();
    cube.eo.random_state();

    return cube;
}

bool operator==(const CubieCube &cc1, const CubieCube &cc2) {
    if (cc1.cp != cc2.cp) return false;
    if (cc1.co != cc2.co) return false;
    if (cc1.ep != cc2.ep) return false;
    if (cc1.eo != cc2.eo) return false;
    return true;
}
}; // namespace Cube 333