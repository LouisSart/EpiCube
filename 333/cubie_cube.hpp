#pragma once
#include <array>
#include <cstdlib>  // srand, rand
#include <ctime>    // time
#include <iostream>
#include <set>
#include <vector>

// #include "algorithm.hpp"   // scramble
// #include "coordinate.hpp"  // ipow
#include "../lib/utils.hpp"
#include "../lib/permutation.hpp"
#include "cubie.hpp"
#include "move.hpp"      // moves and rotations
// #include "symmetry.hpp"  // symmetry components
// #include "utils.hpp"     // print_array

namespace Cube333{
struct CubieCube {
    Permutation<NC> cp;
    Orientation<NC, 3> co;
    Permutation<NE> ep;
    Orientation<NE, 2> eo;

    CubieCube();
    CubieCube(const Sequence<Move> &);
    CubieCube(const std::string &);
    
    void corner_apply(const Move&);
    void edge_apply(const Move&);
    void apply(const Move&);
    void show() const;

    // void apply(const CubieCube &&cc) { apply(cc); }

    // void apply(const Move &);

    void apply(const Sequence<Move> &);

    // void apply(const Skeleton &);

    // void sym_apply(const Move &, const unsigned &);

    // void sym_apply(const Algorithm &, const unsigned &);

    // void sym_apply(const Skeleton &, const unsigned &);

    CubieCube get_conjugate(const CubieCube&) const;

    // CubieCube get_anti_conjugate(const unsigned &sym_index) const;

    // void RL_mirror() {
    //     // Apply the RL mirroring symmetry. This cannot be
    //     // applied as a legal cube manipulation

    //     static CubieCube S_LR{{URF, ULF, ULB, URB, DRF, DLF, DLB, DRB},
    //                           {0, 0, 0, 0, 0, 0, 0, 0},
    //                           {UF, UL, UB, UR, RF, LF, LB, RB, DF, DL, DB, DR},
    //                           {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};

    //     apply(S_LR);
    //     for (unsigned c = ULF; c < NC; ++c) {
    //         co[c] = (3 - co[c]) % 3;
    //     }
    //     for (unsigned e = UF; e < NE; ++e) {
    //         eo[e] = (2 - eo[e]) % 2;
    //     }
    // }

    CubieCube get_inverse() const {
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

    // bool operator==(const CubieCube &other) {
    //     for (unsigned c = ULF; c < NC; ++c) {
    //         if (cp[c] != other.cp[c]) {
    //             return false;
    //         }
    //         if (co[c] != other.co[c]) {
    //             return false;
    //         }
    //     }
    //     for (unsigned e = UF; e < NE; ++e) {
    //         if (ep[e] != other.ep[e]) {
    //             return false;
    //         }
    //         if (eo[e] != other.eo[e]) {
    //             return false;
    //         }
    //     }
    //     return true;
    // }

    // bool operator!=(const CubieCube &other) {return !(*this == other);}

    // CubieCube &operator=(const CubieCube &other) {
    //     if (this == &other)  // self assignment guard
    //         return *this;

    //     for (unsigned c = ULF; c < NC; ++c) {
    //         cp[c] = other.cp[c];
    //         co[c] = other.co[c];
    //     }
    //     for (unsigned e = UF; e < NE; ++e) {
    //         ep[e] = other.ep[e];
    //         eo[e] = other.eo[e];
    //     }
    //     return *this;
    // }

    // int corner_parity() const {
    //     std::set<unsigned> checked;
    //     std::vector<int> cycle_sizes{0};

    //     for (unsigned k = ULF; k < NC; ++k) {
    //         while (checked.find(k) == checked.end()) {
    //             checked.insert(k);
    //             ++cycle_sizes.back();
    //             k = cp[k];
    //         }
    //         cycle_sizes.emplace_back(0);
    //     }

    //     unsigned number_of_swaps = 0;
    //     for (auto k : cycle_sizes) {
    //         if (k != 0) {
    //             number_of_swaps += (k - 1);
    //         }
    //     }
    //     return ipow(-1, number_of_swaps);
    // }

    // int edge_parity() const {
    //     std::set<unsigned> checked;
    //     std::vector<int> cycle_sizes{0};

    //     for (unsigned k = UF; k < NE; ++k) {
    //         while (checked.find(k) == checked.end()) {
    //             checked.insert(k);
    //             ++cycle_sizes.back();
    //             k = ep[k];
    //         }
    //         cycle_sizes.emplace_back(0);
    //     }

    //     int number_of_swaps = 0;
    //     for (auto k : cycle_sizes) {
    //         if (k != 0) {
    //             number_of_swaps += (k - 1);
    //         }
    //     }
    //     return ipow(-1, number_of_swaps);
    // }

    // bool has_consistent_co() const {
    //     int co_sum = 0;
    //     for (unsigned c = ULF; c < NC; ++c) {
    //         co_sum += co[c];
    //     }
    //     if (co_sum % 3 != 0) {
    //         return false;
    //     }
    //     return true;
    // }

    // bool has_consistent_eo() const {
    //     int eo_sum = 0;
    //     for (unsigned e = UF; e < NE; ++e) {
    //         eo_sum += eo[e];
    //     }
    //     if (eo_sum % 2 != 0) {
    //         return false;
    //     }
    //     return true;
    // }

    bool is_solvable() const;
    bool is_solved() const;
    static CubieCube random_state();
};

bool operator==(const CubieCube &, const CubieCube &);
CubieCube operator*(const CubieCube &, const CubieCube &);

// CubieCube rotation_cc[N_ELEM_SYM - 1]{
//     /* S_URF */
//     CubieCube{{DRF, URF, ULF, DLF, DRB, URB, ULB, DLB},
//               {2, 1, 2, 1, 1, 2, 1, 2},
//               {RF, UF, LF, DF, DR, UR, UL, DL, RB, UB, LB, DB},
//               {0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1}},

//     /* y */
//     CubieCube{{URF, URB, ULB, ULF, DRF, DRB, DLB, DLF},
//               {0, 0, 0, 0, 0, 0, 0, 0},
//               {UR, UB, UL, UF, RF, RB, LB, LF, DR, DB, DL, DF},
//               {0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0}},

//     /* z2 */
//     CubieCube{{DRF, DLF, DLB, DRB, URF, ULF, ULB, URB},
//               {0, 0, 0, 0, 0, 0, 0, 0},
//               {DF, DL, DB, DR, RF, LF, LB, RB, UF, UL, UB, UR},
//               {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
// };

// CubieCube CubieCube::get_conjugate(const unsigned &sym_index) const {
//     // Return the CubieCube S^-1 * cc * S where cc is *this and
//     // S is the symmetry number sym_index

//     // S = S_LR^(c_lr) * S_z2^(c_z2) * S_y^(c_y) * S_URF^(c_surf)
//     auto [c_surf, c_y, c_z2, c_lr] = symmetry_index_to_num(sym_index);

//     CubieCube S, S_inv;

//     for (unsigned k_z2 = 0; k_z2 < c_z2; ++k_z2) {
//         S.apply(rotation_cc[2]);
//     }
//     for (unsigned k_y = 0; k_y < c_y; ++k_y) {
//         S.apply(rotation_cc[1]);
//     }
//     for (unsigned k_surf = 0; k_surf < c_surf; ++k_surf) {
//         S.apply(rotation_cc[0]);
//     }
//     S_inv = S.get_inverse();

//     CubieCube ret;

//     ret.apply(S_inv);
//     for (unsigned k_lr = 0; k_lr < c_lr; ++k_lr) {
//         ret.RL_mirror();
//     }
//     ret.apply(*this);
//     for (unsigned k_lr = 0; k_lr < c_lr; ++k_lr) {
//         // LR_mirror cannot be performed like a move application
//         ret.RL_mirror();
//     }
//     ret.apply(S);

//     return ret;
// }

// CubieCube CubieCube::get_anti_conjugate(const unsigned &sym_index) const {
//     // Return the CubieCube S * cc * S^-1 where cc is *this and
//     // S is the symmetry number sym_index

//     // S = S_LR^(c_lr) * S_z2^(c_z2) * S_y^(c_y) * S_URF^(c_surf)
//     auto [c_surf, c_y, c_z2, c_lr] = symmetry_index_to_num(sym_index);

//     CubieCube S, S_inv;

//     for (unsigned k_z2 = 0; k_z2 < c_z2; ++k_z2) {
//         S.apply(rotation_cc[2]);
//     }
//     for (unsigned k_y = 0; k_y < c_y; ++k_y) {
//         S.apply(rotation_cc[1]);
//     }
//     for (unsigned k_surf = 0; k_surf < c_surf; ++k_surf) {
//         S.apply(rotation_cc[0]);
//     }
//     S_inv = S.get_inverse();

//     CubieCube ret;

//     for (unsigned k_lr = 0; k_lr < c_lr; ++k_lr) {
//         ret.RL_mirror();
//     }
//     ret.apply(S);
//     ret.apply(*this);
//     ret.apply(S_inv);
//     for (unsigned k_lr = 0; k_lr < c_lr; ++k_lr) {
//         // LR_mirror cannot be performed like a move application
//         ret.RL_mirror();
//     }
//     return ret;
// }

// CubieCube move_cc[N_HTM_MOVES]{
//     /*U*/
//     CubieCube{{URF, URB, ULB, ULF, DLF, DRF, DRB, DLB},
//               {0, 0, 0, 0, 0, 0, 0, 0},
//               {UR, UB, UL, UF, LF, RF, RB, LB, DF, DR, DB, DL},
//               {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
//     /*U2*/
//     CubieCube{{URB, ULB, ULF, URF, DLF, DRF, DRB, DLB},
//               {0, 0, 0, 0, 0, 0, 0, 0},
//               {UB, UL, UF, UR, LF, RF, RB, LB, DF, DR, DB, DL},
//               {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
//     /*U'*/
//     CubieCube{{ULB, ULF, URF, URB, DLF, DRF, DRB, DLB},
//               {0, 0, 0, 0, 0, 0, 0, 0},
//               {UL, UF, UR, UB, LF, RF, RB, LB, DF, DR, DB, DL},
//               {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
//     /*D*/
//     CubieCube{{ULF, URF, URB, ULB, DLB, DLF, DRF, DRB},
//               {0, 0, 0, 0, 0, 0, 0, 0},
//               {UF, UR, UB, UL, LF, RF, RB, LB, DL, DF, DR, DB},
//               {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
//     /*D2*/
//     CubieCube{{ULF, URF, URB, ULB, DRB, DLB, DLF, DRF},
//               {0, 0, 0, 0, 0, 0, 0, 0},
//               {UF, UR, UB, UL, LF, RF, RB, LB, DB, DL, DF, DR},
//               {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
//     /*D'*/
//     CubieCube{{ULF, URF, URB, ULB, DRF, DRB, DLB, DLF},
//               {0, 0, 0, 0, 0, 0, 0, 0},
//               {UF, UR, UB, UL, LF, RF, RB, LB, DR, DB, DL, DF},
//               {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
//     /*R*/
//     CubieCube{{ULF, DRF, URF, ULB, DLF, DRB, URB, DLB},
//               {0, 2, 1, 0, 0, 1, 2, 0},
//               {UF, RF, UB, UL, LF, DR, UR, LB, DF, RB, DB, DL},
//               {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
//     /*R2*/
//     CubieCube{{ULF, DRB, DRF, ULB, DLF, URB, URF, DLB},
//               {0, 0, 0, 0, 0, 0, 0, 0},
//               {UF, DR, UB, UL, LF, RB, RF, LB, DF, UR, DB, DL},
//               {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
//     /*R'*/
//     CubieCube{{ULF, URB, DRB, ULB, DLF, URF, DRF, DLB},
//               {0, 2, 1, 0, 0, 1, 2, 0},
//               {UF, RB, UB, UL, LF, UR, DR, LB, DF, RF, DB, DL},
//               {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
//     /*L*/
//     CubieCube{{ULB, URF, URB, DLB, ULF, DRF, DRB, DLF},
//               {1, 0, 0, 2, 2, 0, 0, 1},
//               {UF, UR, UB, LB, UL, RF, RB, DL, DF, DR, DB, LF},
//               {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
//     /*L2*/
//     CubieCube{{DLB, URF, URB, DLF, ULB, DRF, DRB, ULF},
//               {0, 0, 0, 0, 0, 0, 0, 0},
//               {UF, UR, UB, DL, LB, RF, RB, LF, DF, DR, DB, UL},
//               {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
//     /*L'*/
//     CubieCube{{DLF, URF, URB, ULF, DLB, DRF, DRB, ULB},
//               {1, 0, 0, 2, 2, 0, 0, 1},
//               {UF, UR, UB, LF, DL, RF, RB, UL, DF, DR, DB, LB},
//               {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
//     /*F*/
//     CubieCube{{DLF, ULF, URB, ULB, DRF, URF, DRB, DLB},
//               {2, 1, 0, 0, 1, 2, 0, 0},
//               {LF, UR, UB, UL, DF, UF, RB, LB, RF, DR, DB, DL},
//               {1, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0}},
//     /*F2*/
//     CubieCube{{DRF, DLF, URB, ULB, URF, ULF, DRB, DLB},
//               {0, 0, 0, 0, 0, 0, 0, 0},
//               {DF, UR, UB, UL, RF, LF, RB, LB, UF, DR, DB, DL},
//               {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
//     /*F'*/
//     CubieCube{{URF, DRF, URB, ULB, ULF, DLF, DRB, DLB},
//               {2, 1, 0, 0, 1, 2, 0, 0},
//               {RF, UR, UB, UL, UF, DF, RB, LB, LF, DR, DB, DL},
//               {1, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0}},
//     /*B*/
//     CubieCube{{ULF, URF, DRB, URB, DLF, DRF, DLB, ULB},
//               {0, 0, 2, 1, 0, 0, 1, 2},
//               {UF, UR, RB, UL, LF, RF, DB, UB, DF, DR, LB, DL},
//               {0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 1, 0}},
//     /*B2*/
//     CubieCube{{ULF, URF, DLB, DRB, DLF, DRF, ULB, URB},
//               {0, 0, 0, 0, 0, 0, 0, 0},
//               {UF, UR, DB, UL, LF, RF, LB, RB, DF, DR, UB, DL},
//               {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
//     /*B'*/
//     CubieCube{{ULF, URF, ULB, DLB, DLF, DRF, URB, DRB},
//               {0, 0, 2, 1, 0, 0, 1, 2},
//               {UF, UR, LB, UL, LF, RF, UB, DB, DF, DR, RB, DL},
//               {0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 1, 0}}};

// // void CubieCube::apply(const Move &m) { apply(move_cc[m]); }

// // void CubieCube::apply(const Algorithm &alg) {
// //     if (alg.inv_flag) {
// //         auto cc = CubieCube();
// //         for (auto it = alg.sequence.rbegin(); it != alg.sequence.rend(); ++it) {
// //             cc.apply(inverse_of_HTM_Moves[*it]);
// //         }
// //         cc.apply(*this);
// //         *this = cc;
// //     } else {
// //         for (const Move &m : alg.sequence) {
// //             apply(m);
// //         }
// //     }
// // }

// // void CubieCube::apply(const Skeleton &skel) {
// //     for (const StepAlgorithm &step : skel) {
// //         apply(step);
// //     }
// // }

// // void CubieCube::sym_apply(const Move &m, const unsigned &s) {
// //     apply(move_conj(m, s));
// // }

// // void CubieCube::sym_apply(const Algorithm &alg, const unsigned &s) {
// //     if (alg.inv_flag) {
// //         auto cc = CubieCube();
// //         for (auto it = alg.sequence.rbegin(); it != alg.sequence.rend(); ++it) {
// //             cc.apply(move_conj(inverse_of_HTM_Moves[*it], s));
// //         }
// //         cc.apply(*this);
// //         *this = cc;
// //     } else {
// //         for (const Move &m : alg.sequence) {
// //             apply(move_conj(m, s));
// //         }
// //     }
// // }

// // void CubieCube::sym_apply(const Skeleton &skel, const unsigned &s) {
// //     for (const StepAlgorithm &step : skel) {
// //         sym_apply(step, s);
// //     }
// // }
}; // namespace Cube333