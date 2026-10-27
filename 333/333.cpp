#include <iostream>       // std::cout << Move
#include <sstream>        // std::isstream
#include "move.hpp"
#include "cubie_cube.hpp"
#include "defs.hpp"
#include "symmetry.hpp"

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

Sequence<Move> make_sequence(std::string str_in) {
    Sequence<Move> ret;

    if (str_in[0] == '(') {  // Turn on inverse flag if alg starts with '('
        assert(str_in.back() == ')');
        ret.inv_flag = true;
        str_in = std::string(str_in.begin() + 1, str_in.end() - 1);
    }

    std::istringstream f(str_in);
    std::string s;    
    while (std::getline(f, s, ' ')) {
        ret.push_back(HTM::str_to_move(s));
    }
    return ret;
};

CubieCube::CubieCube(){};
CubieCube::CubieCube(
    const Permutation<NC> &cp_in, const Orientation<NC, 3> &co_in,
    const Permutation<NE> &ep_in, const Orientation<NE, 2> &eo_in) :
    cp{cp_in}, co{co_in}, ep{ep_in}, eo{eo_in}{}
CubieCube::CubieCube(const Sequence<Move> &scramble) { apply(scramble); }
CubieCube::CubieCube(const std::string &scramble) { apply(make_sequence(scramble)); }

void CubieCube::corner_apply(const Move& m) {
    cp.compose(CPs[m]);
    co = (co * CPs[m]) + COs[m];
}

void CubieCube::edge_apply(const Move& m) {
    ep.compose(EPs[m]);
    eo = (eo * EPs[m]) + EOs[m];
}

void CubieCube::apply(const Move& m) {
    corner_apply(m);
    edge_apply(m);
}

void CubieCube::apply(const Sequence<Move> &seq) {
    if (!seq.inv_flag) {
        for (Move m : seq) {
            apply(m);
        }
    } else {
        CubieCube tmp;
        for (Move m : seq) {
            tmp.apply(m);
        }
        *this = tmp.get_inverse() * (*this);
    }
};

CubieCube CubieCube::get_conjugate(const CubieCube &conj) const {
    return conj.get_inverse() * (*this) * conj;
}

CubieCube CubieCube::get_inverse() const {
    // return the inverse permutation
    CubieCube inverse;
    for (unsigned c = ULF; c < NC; ++c) {
        inverse.cp[cp[c]] = (unsigned)c;
        inverse.co[cp[c]] = (3 - co[c]) % 3;
    };
    for (unsigned e = UF; e < NE; ++e) {
        inverse.ep[ep[e]] = (unsigned)e;
        inverse.eo[ep[e]] = (2 - eo[e]) % 2;
    };
    return inverse;
}

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

bool CubieCube::is_solvable() const {
    if (!co.is_even()) return false;
    if (!eo.is_even()) return false;
    if (cp.parity() != ep.parity()) return false;
    return true;
}

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
    cube.co.random_state(true);
    cube.ep.random_state();
    cube.eo.random_state(true);
    if (cube.ep.parity() != cube.cp.parity()) swap(0, 1, cube.ep);
    return cube;
}

bool operator==(const CubieCube &cc1, const CubieCube &cc2) {
    if (cc1.cp != cc2.cp) return false;
    if (cc1.co != cc2.co) return false;
    if (cc1.ep != cc2.ep) return false;
    if (cc1.eo != cc2.eo) return false;
    return true;
}

CubieCube operator*(const CubieCube &c1, const CubieCube &c2) {
    CubieCube ret;
    ret.cp = c1.cp * c2.cp;
    ret.co = (c1.co * c2.cp) + c2.co;
    ret.ep = c1.ep * c2.ep;
    ret.eo = (c1.eo * c2.ep) + c2.eo;
    return ret;
}

CubieCube operator^(const CubieCube &c1, const unsigned n) {
    CubieCube ret;
    for (unsigned k = 0; k < n; ++k) {
        ret = ret * c1;
    }
    return ret;
}

// SYMMETRY INDEX
unsigned symmetry_index(const unsigned c_surf, const unsigned c_y,
                        const unsigned c_z2, const unsigned c_lr) {
    // Assign a unique index to each combination of symmetries

    assert(c_surf < N_SURF);
    assert(c_y < N_Y);
    assert(c_z2 < N_Z2);
    assert(c_lr < N_LR);

    unsigned coord = c_lr + N_LR * (c_z2 + N_Z2 * (c_y + N_Y * (c_surf)));

    assert(coord < N_SYM);
    return coord;
}

std::tuple<unsigned, unsigned, unsigned, unsigned> symmetry_index_to_num(const unsigned index) {
    // Retrieve the symmetry components from the given index

    assert(index < N_SYM);

    unsigned div = index;
    unsigned c_lr = div % N_LR;
    div = div / N_LR;
    unsigned c_z2 = div % N_Z2;
    div = div / N_Z2;
    unsigned c_y = div % N_Y;
    div = div / N_Y;
    unsigned c_surf = div;

    assert(c_surf < N_SURF);
    assert(c_y < N_Y);
    assert(c_z2 < N_Z2);
    assert(c_lr < N_LR);

    return std::make_tuple(c_surf, c_y, c_z2, c_lr);
}

auto get_move_permutation(const unsigned &sym_index) {
    Permutation<N_HTM_MOVES> ret = {U, U2, U3, D, D2, D3, R, R2, R3,
                                         L, L2, L3, F, F2, F3, B, B2, B3};
    auto [c_surf, c_y, c_z2, c_lr] = symmetry_index_to_num(sym_index);

    for (unsigned k_lr = 0; k_lr < c_lr; ++k_lr) {
        ret = LR_mirror_move_conj * ret;
    }
    for (unsigned k_z2 = 0; k_z2 < c_z2; ++k_z2) {
        ret = z2_move_conj * ret;
    }
    for (unsigned k_y = 0; k_y < c_y; ++k_y) {
        ret = y_move_conj * ret;
    }
    for (unsigned k_surf = 0; k_surf < c_surf; ++k_surf) {
        ret = S_URF_move_conj * ret;
    }

    return ret;
}

Move move_conj(const Move &m, const unsigned &s) {
    static auto move_conj_table = [] {
        std::array<Move, N_SYM * N_HTM_MOVES> ret;
        for (unsigned s = 0; s < N_SYM; ++s) {
            for (Move m : HTM::moves) {
                auto move_perm = get_move_permutation(s);
                ret[s * N_HTM_MOVES + m] = (Move)move_perm[m];
            }
        }
        return ret;
    }();
    return move_conj_table[s * N_HTM_MOVES + m];
}

Move move_anti_conj(const Move &m, const unsigned &s) {
    static auto move_anti_conj_table = [] {
        std::array<Move, N_SYM * N_HTM_MOVES> ret;
        for (unsigned s = 0; s < N_SYM; ++s) {
            for (Move m : HTM::moves) {
                auto move_perm = get_move_permutation(s);
                ret[s * N_HTM_MOVES + (Move)move_perm[m]] = m;
            }
        }
        return ret;
    }();
    return move_anti_conj_table[s * N_HTM_MOVES + m];
}

Sequence<Move> symmetrize(const Sequence<Move> &alg, const unsigned &sym_index) {
    // Return the algorithm that is the conjugation of alg by the symmetry
    // with index sym_index

    Sequence<Move> ret;
    for (const Move &m : alg) {
        ret.push_back(move_conj(m, sym_index));
    }
    return ret;
}

Sequence<Move> anti_symmetrize(const Sequence<Move> &alg, const unsigned &sym_index) {
    // Return the algorithm that is the anti-conjugation of
    // alg by the symmetry with index sym_index

    Sequence<Move> ret;
    for (const Move &m : alg) {
        ret.push_back(move_anti_conj(m, sym_index));
    }
    return ret;
}

// SYMMETRY COMBINATION
// [U, D, R, L, F, B]
// [0, 1, 2, 3, 4, 5]
static Permutation<6> ccurf{4, 5, 0, 1, 2, 3};
static Permutation<6> ccy{0, 1, 5, 4, 2, 3};
static Permutation<6> ccz2{1, 0, 3, 2, 4, 5};
static Permutation<6> ccrl{0, 1, 3, 2, 4, 5};

void apply_sym(std::string &cc, const unsigned &sym) {

    auto [curf, cy, cz2, crl] = symmetry_index_to_num(sym);

    for (unsigned krl = 0; krl < crl; ++krl) {
        permute(cc, ccrl);
    }
    for (unsigned kz2 = 0; kz2 < cz2; ++kz2) {
        permute(cc, ccz2);
    }
    for (unsigned ky = 0; ky < cy; ++ky) {
        permute(cc, ccy);
    }
    for (unsigned kurf = 0; kurf < curf; ++kurf) {
        permute(cc, ccurf);
    }
}

auto make_sym_comb_table() {
    std::string cc;
    std::unordered_map<std::string, unsigned> perm_to_sym;

    for (unsigned s = 0; s < N_SYM; ++s) {
        cc = "UDRLFB";
        apply_sym(cc, s);
        perm_to_sym.insert({cc, s});
    }

    std::array<unsigned, N_SYM * N_SYM> ret;
    for (unsigned s1 = 0; s1 < N_SYM; ++s1) {
        for (unsigned s2 = 0; s2 < N_SYM; ++s2) {
            cc = "UDRLFB";
            apply_sym(cc, s1);
            apply_sym(cc, s2);
            unsigned s = perm_to_sym[cc];

            for (Move m : HTM::moves) {
                assert(move_conj(move_conj(m, s1), s2) == move_conj(m, s));
            }

            ret[s1 * N_SYM + s2] = s;
        }
    }
    return ret;
}

unsigned sym_combine(const unsigned &s1, const unsigned &s2) {
    // return the coordinate of the symmetry s = s1 o s2

    // Takes 3ms on my computer
    static const std::array<unsigned, N_SYM * N_SYM> sym_comb_table = make_sym_comb_table();
    // I don't think its worth making a local copy of the table

    return sym_comb_table[s1 * N_SYM + s2];
}

std::array<CubieCube, N_SYM> make_cubie_cube_sym_table() {
    // Build the table of CubieCubes that represent each of the
    // 48 symmetries of the 3x3x3

    static CubieCube surf_cc{SURF_cp, SURF_co, SURF_ep, SURF_eo};
    static CubieCube y_cc{y_cp, y_co, y_ep, y_eo};
    static CubieCube z2_cc{z2_cp, z2_co, z2_ep, z2_eo};
    static CubieCube rl_cc{rl_cp, rl_co, rl_ep, rl_eo};

    std::array<CubieCube, N_SYM> table;
    for (unsigned c = 0; c < N_SYM; ++c) {
        auto [curf, cy, cz2, crl] = symmetry_index_to_num(c);
        CubieCube sym_cc;
        sym_cc = sym_cc * (rl_cc ^ crl);
        sym_cc = sym_cc * (z2_cc ^ cz2);
        sym_cc = sym_cc * (y_cc ^ cy);
        sym_cc = sym_cc * (surf_cc ^ curf);
        table[c] = sym_cc;
        if (c > 0) assert(!sym_cc.is_solved());
    }
    return table;
}

CubieCube CubieCube::get_conjugate(const unsigned &c) const {
    static auto sym_cubes = make_cubie_cube_sym_table();
    CubieCube sym_cc = sym_cubes[c];
    bool mirror = c % N_LR; // true if symmetry includes mirroring
    if (c > 0) assert(!sym_cc.is_solved());
    CubieCube ret = sym_cc.get_inverse();
    // RL mirroring doesn't behave like a legal move application
    if (mirror) ret.co.complement();
    ret = ret * (*this);
    if (mirror) ret.co.complement();
    ret = ret * sym_cc;
    return ret;
}
}; // namespace Cube 333