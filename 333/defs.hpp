#include "../lib/permutation.hpp"

// This is the file for defining changes in permutation and
// orientations for corners and edges when applying
// elementary moves and rotations

namespace Cube333 {

// CORNERS : permutations
static constexpr Permutation<NC> SURF_cp {DRF, URF, ULF, DLF, DRB, URB, ULB, DLB};
static constexpr Permutation<NC> y_cp {URF, URB, ULB, ULF, DRF, DRB, DLB, DLF};
static constexpr Permutation<NC> z2_cp {DRF, DLF, DLB, DRB, URF, ULF, ULB, URB};
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

// CORNERS : orientations
static constexpr Orientation<NC, 3> SURF_co {2, 1, 2, 1, 1, 2, 1, 2};
static constexpr Orientation<NC, 3> SURF2_co = (SURF_co * SURF_cp) + SURF_co;
static constexpr Orientation<NC, 3> R_co = (SURF2_co * U_cp * SURF_cp) + SURF_co;
static constexpr Orientation<NC, 3> F_co = (SURF_co * U_cp * SURF_cp.get_inverse()) + SURF2_co;
static constexpr Orientation<NC, 3> L_co = R_co * z2_cp;
static constexpr Orientation<NC, 3> B_co = F_co * y_cp * y_cp;

static constexpr Orientation<NC, 3> COs[N_HTM_MOVES] {
    {0, 0, 0, 0, 0, 0, 0, 0},               // U
    {0, 0, 0, 0, 0, 0, 0, 0},               // U2
    {0, 0, 0, 0, 0, 0, 0, 0},               // U'
    {0, 0, 0, 0, 0, 0, 0, 0},               // D
    {0, 0, 0, 0, 0, 0, 0, 0},               // D2
    {0, 0, 0, 0, 0, 0, 0, 0},               // D'
    R_co,                                   // R
    (R_co * R_cp) + R_co,                   // R2
    ((R_co * R_cp) + R_co) * R_cp + R_co,   // R'
    L_co,                                   // L
    (L_co * L_cp) + L_co,                   // L2
    ((L_co * L_cp) + L_co) * L_cp + L_co,   // L'
    F_co,                                   // F
    (F_co * F_cp) + F_co,                   // F2
    ((F_co * F_cp) + F_co) * F_cp + F_co,   // F'
    B_co,                                   // B
    (B_co * B_cp) + B_co,                   // B2
    ((B_co * B_cp) + B_co) * B_cp + B_co,   // B'
};

// EDGES : permutations
static constexpr Permutation<NE> SURF_ep {RF, UF, LF, DF, DR, UR, UL, DL, RB, UB, LB, DB};
static constexpr Permutation<NE> y_ep {UR, UB, UL, UF, RF, RB, LB, LF, DR, DB, DL, DF};
static constexpr Permutation<NE> z2_ep {DF, DL, DB, DR, RF, LF, LB, RB, UF, UL, UB, UR};
static constexpr Permutation<NE> RL_mirror_ep {UF, UL, UB, UR, RF, LF, LB, RB, DF, DL, DB, DR};

static constexpr Permutation<NE> U_ep {UR, UB, UL, UF, LF, RF, RB, LB, DF, DR, DB, DL};
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

// EDGES : orientations
static constexpr Orientation<NE> SURF_eo {0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1};
static constexpr Orientation<NE> SURF2_eo = (SURF_eo * SURF_ep) + SURF_eo;
static constexpr Orientation<NE> R_eo = (SURF2_eo * U_ep * SURF_ep) + SURF_eo;
static constexpr Orientation<NE> F_eo = (SURF_eo * U_ep * SURF_ep.get_inverse()) + SURF2_eo;
static constexpr Orientation<NE> L_eo = R_eo * z2_ep;
static constexpr Orientation<NE> B_eo = F_eo * y_ep * y_ep;

static constexpr Orientation<NE> EOs[N_HTM_MOVES] {
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   // U
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   // U2
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   // U'
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   // D
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   // D2
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   // D'
    R_eo,                                   // R
    (R_eo * R_ep) + R_eo,                   // R2
    ((R_eo * R_ep) + R_eo) * R_ep + R_eo,   // R'
    L_eo,                                   // L
    (L_eo * L_ep) + L_eo,                   // L2
    ((L_eo * L_ep) + L_eo) * L_ep + L_eo,   // L'
    F_eo,                                   // F
    (F_eo * F_ep) + F_eo,                   // F2
    ((F_eo * F_ep) + F_eo) * F_ep + F_eo,   // F'
    B_eo,                                   // B
    (B_eo * B_ep) + B_eo,                   // B2
    ((B_eo * B_ep) + B_eo) * B_ep + B_eo,   // B'
};

// Let c be a given permutation of the cube
// Given a symmetry S, let conj be the function:

// conj(c, S) = S^-1 * c * S

// If we were to work on a coordinate level and we wanted
// to compute the coordinate for:

// conj(c, S) * m, where m is a legal move

// we would face an issue because the move table
// can only transform the coordinate that represents c
// but not its conjugation. However:
// conj(c, S) * m = S^-1 * c * S * m
//                = S^-1 * c * S * S^-1 * S * m * S^-1 * S
//                = S^-1 * (c * S * m * S^-1) * S
//                = conj(c * conj(m, S^-1), S)
// So we only need to store the conjugations for elementary moves
// This file is where the translation of moves after conjugation
// by a symmetry is computed

constexpr Permutation<N_HTM_MOVES> S_URF_move_conj {R, R2, R3, L, L2, L3, F, F2, F3, B, B2, B3, U, U2, U3, D, D2, D3};
constexpr Permutation<N_HTM_MOVES> y_move_conj {U, U2, U3, D, D2, D3, F, F2, F3, B, B2, B3, L, L2, L3, R, R2, R3};
constexpr Permutation<N_HTM_MOVES> z2_move_conj {D, D2, D3, U, U2, U3, L, L2, L3, R, R2, R3, F, F2, F3, B, B2, B3};
constexpr Permutation<N_HTM_MOVES> LR_mirror_move_conj{U3, U2, U, D3, D2, D, L3, L2, L, R3, R2, R, F3, F2, F, B3, B2, B};
};