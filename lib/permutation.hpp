#pragma once
#include <array>
#include <set>
#include <cassert>
#include <algorithm>
#include "utils.hpp"

constexpr unsigned ipow(unsigned k, unsigned n) {
    // unsigned integer power
    // computes ret = k^n
    unsigned ret{1};
    for (unsigned i = 0; i < n; i++) {
        ret *= k;
    }
    return ret;
};

constexpr auto factorial_table = [] {
    std::array<unsigned, 13> arr = {};
    arr[0] = 1;
    for (unsigned n = 1; n <= 12; ++n) {
        arr[n] = n * arr[n - 1];
    }
    return arr;
}();

constexpr unsigned factorial(unsigned n) {
    assert(n <= 12);
    return factorial_table[n];
}

constexpr auto binomial_table = [] {
    // Using constexpr lambda to fill up binomial table
    // at compile time
    std::array<unsigned, (13) * (13)> arr = {};
    for (unsigned n = 0; n <= 12; ++n) {
        for (unsigned k = 0; k <= 12; ++k) {
            if (n < k) {
                arr[n * 13 + k] = 0;
            } else if (k == 0 || k == n) {
                arr[n * 13 + k] = 1;
            } else {
                arr[n * 13 + k] =
                    arr[(n - 1) * 13 + k - 1] + arr[(n - 1) * 13 + k];
            }
        }
    }
    return arr;
}();

constexpr unsigned binomial(unsigned n, unsigned k) {
    // The function just does a lookup in the table for better performance
    assert(n < 13 && k < 13);  // "Binomial numbers computed up to n=12"
    return binomial_table[n * 13 + k];
}


template <std::size_t n>
unsigned layout_index(const std::array<unsigned, n> &layout, unsigned r) {
    // n: number of positions
    // r: number of pieces
    assert(r <= n);
    assert(0 < n);
    unsigned t = 0;
    for (unsigned i = n - 1; i > 0; --i) {
        if (layout[i] == 1) {
            t += binomial(i, r);
            r -= 1;
        }
    }
    return t;
}

template <std::size_t n>
void layout_from_index(unsigned c, std::array<unsigned, n> &layout,
                       unsigned r) {
    // n: number of positions
    // r: number of pieces
    assert(r <= n);
    assert(c < binomial(n, r));
    for (auto i = n - 1; i < n; --i) {
        if (c >= binomial(i, r)) {
            c = c - binomial(i, r);
            layout[i] = 1;
            r = r - 1;
        } else {
            layout[i] = 0;
        }
    }
}

template<std::size_t N>
void swap(const unsigned &i, const unsigned &j, std::array<unsigned, N> &a){
    assert(i < N);
    assert(j < N);
    unsigned buf = a[i];
    a[i] = a[j];
    a[j] = buf;
}

template<std::size_t N>
void random_shuffle(std::array<unsigned, N> &a) {
    // Fisher-Yates shuffle
    srand(time(0));
    for (unsigned i = 0; i < N - 1; ++i) {
        unsigned j = i + rand() % (N - i);
        swap(i, j, a);
    }
}

template <unsigned N, unsigned M>
struct Layout : std::array<unsigned, N> {
    // holds the positions of M elements amongst
    // N positions regardless of order
    static constexpr unsigned filled = 1;
    static constexpr unsigned empty = 0;

    Layout() { // Constructeur sans arguments
        static_assert(M < N);
        reset();
    }

    Layout(const unsigned &c) { // Constructeur from index
        this->set_from_index(c);
    }

    Layout(const std::array<unsigned, N> &p, const std::array<unsigned, M> &pieces) {
        for (unsigned k = 0; k < N; ++k) {
            if (std::find(pieces.begin(), pieces.end(), p[k]) != pieces.end()){
                this->operator[](k) = 1;
            } else {
                this->operator[](k) = 0;
            }
        }
    }

    void reset() {
        this->fill(empty);
        for (unsigned k = 0; k < M; ++k){
            this->operator[](k) = filled;
        }
    }

    void random_state() {
        random_shuffle(*this);
    }

    constexpr unsigned cardinality() const {
        return CARD;
    }

    static constexpr unsigned CARD = binomial(N, M);

    unsigned index() const {
        return layout_index(*this, M);
    }

    void set_from_index(const unsigned &c) {
        layout_from_index(c, *this, M);
    }
};

template<unsigned N>
struct Permutation : std::array<unsigned, N> {

    constexpr Permutation() { // Constructeur sans arguments
        for (unsigned k = 0; k < N; ++k){
            this->operator[](k) = k;
        }
    }

    constexpr Permutation(const unsigned &c) { // Constructeur from index
        this->set_from_index(c);
    }

    template<typename... Args>
    constexpr Permutation(Args... args) : std::array<unsigned, N>{{ static_cast<unsigned>(args)... }} {} // Constructeur par brace-enclosed

    void random_state(const bool even = false) {
        random_shuffle(*this);
        if (even && parity() != 0) {
            swap(N - 1, N - 2, *this);
        }
    }

    bool is_solved() const {
        for (unsigned k = 0; k < N; ++k){
            if (this->operator[](k) != k) return false;
        }
        return true;
    }

    static constexpr unsigned cardinality() { // CHECKME : unneccessary ?
        return CARD;
    }
    static constexpr unsigned CARD = factorial(N);

    unsigned parity() const {
        std::set<unsigned> visited;
        unsigned i = 0, swaps = 0;
        while (visited.size() < N) {
            if (!visited.contains(i)){
                visited.insert(i);
                unsigned buf = (*this)[i];
                while (buf != i) {
                    visited.insert(buf);
                    buf = (*this)[buf];
                    ++swaps;
                }
            }
            ++i;
        }
        return swaps % 2;
    }

    void reset() {
        for (unsigned k = 0; k < N; ++k){
            this->operator[](k) = k;
        }
    }

    constexpr void compose(const Permutation<N> & other) {
        Permutation<N> ret = *this;
        for (unsigned k = 0; k < N; ++k){
            this->operator[](k) = ret[other[k]];
        }
    }

    constexpr Permutation<N> get_conjugate(const Permutation<N> &p) const {
        // return p^-1 o (*this) o p
        Permutation<N> ret = p.get_inverse();
        ret.compose(*this);
        ret.compose(p);
        return ret;
    }

    constexpr Permutation<N> get_inverse() const {
        Permutation<N> ret;
        for (unsigned k = 0; k < N; ++k) {
            ret[(*this)[k]] = k;
        };
        return ret;
    }

    unsigned index(const bool even = false) const {
        // Compute the lexicographic index of the permutation
        static_assert(N > 0); // empty permutations are a problem
        // If the permutation necessarily has even parity,
        // do not encode the information of the last two digits
        unsigned n;
        if (even) {n = N - 1;
            assert(N > 2); // trivial case do not implement
        }
        else n = N;
        
        unsigned t = 0;
        for (unsigned i = 0; i < n - 1; ++i){
            t = t * (N - i);
            for (unsigned j = i; j < N; ++j) {
                if ((*this)[i] > (*this)[j]) t = t + 1;
            }
        }
        return t;
    }

    void set_from_index(unsigned c, const bool even = false) {
        // Reconstruct the permutation having index c
        static_assert(N > 0); // empty permutations are a problem

        if (even) {
            assert(c < CARD / 2);
            assert(N > 2); // trivial case do not implement
            unsigned s = 0;
            (*this)[N - 1] = 1;
            (*this)[N - 2] = 0;
            for (unsigned i = N - 3; i < N; --i) {
                (*this)[i] = (c % (N - i));
                s += (*this)[i];
                c = c / (N - i);
                for (auto j = i + 1; j < N; ++j) {
                    if ((*this)[j] >= (*this)[i]) {
                        (*this)[j] += 1;
                    }
                }
            }
            if (s % 2 == 1) swap(N - 1, N - 2, *this);
        } else {
            assert(c < CARD);
            (*this)[N - 1] = 0;
            for (unsigned i = N - 2; i < N; --i) {
                (*this)[i] = (c % (N - i));
                c = c / (N - i);
                for (auto j = i + 1; j < N; ++j) {
                    if ((*this)[j] >= (*this)[i]) {
                        (*this)[j] += 1;
                    }
                }
            }
        }
    }

    template<unsigned M>
    Permutation<M> get_sub_permutation() const {
        // returns the sub permutation for values
        // 0 to M - 1
        static_assert(M < N);
        Permutation<M> ret;
        auto it = ret.begin();
        for (unsigned k : *this) {
            if (k < M) {
                *it = k;
                ++it;
            }
        }
        return ret;
    }

    template<unsigned M>
    Permutation<M> get_sub_permutation_complement() const {
        // returns the sub permutation for values
        // M to N - 1
        static_assert(M < N);
        Permutation<M> ret;
        auto it = ret.begin();
        for (unsigned k : *this) {
            if (k >= M) {
                *it = k;
                ++it;
            }
        }
        return ret;
    }

    template<long unsigned M>
    Permutation<M> get_sub_permutation(const std::array<unsigned, M> &pieces) const {
        // returns the sub permutation of elements in
        // array pieces
        static_assert(M < N);
        Permutation<M> ret;
        unsigned i = 0;
        for (unsigned k : *this) {
            auto it = std::find(pieces.begin(), pieces.end(), k);
            if (it != pieces.end()) {
                ret[i] = it - pieces.begin();
                ++i;
            }
        }
        return ret;
    }

    template<unsigned M = N / 2>
    std::array<unsigned, 3> split_indices() const {
        // split the Permutation in two splits
        // return the layout coordinate for the first set
        // as well along the local permutation
        // index for each set
        static_assert(M < N);
        Layout<N, M> l;
        for (unsigned i = 0; i < N; ++i) {
            if ((*this)[i] < M) l[i] = 1;
            else l[i] = 0;
        }
        Permutation<M> p1 = get_sub_permutation<M>();
        Permutation<M> p2 = get_sub_permutation_complement<M>();

        return {l.index(), p1.index(), p2.index()};
    }

    template<unsigned M = N / 2>
    void set_from_split_indices(unsigned cl, unsigned c1, unsigned c2) {
        Layout<N, M> l(cl);
        Permutation<M> p1(c1);
        Permutation<N - M> p2(c2);

        unsigned k1 = 0, k2 = 0;
        for (unsigned i = 0; i < N; ++i) {
            if (l[i] == 1) {
                (*this)[i] = p1[k1];
                ++k1;
            } else {
                (*this)[i] = p2[k2] + M;
                ++k2;
            }
        }
    }

    template<long unsigned M>
    unsigned partial_index(const std::array<unsigned, M> &pieces) const {
        static_assert(M < N);
        Layout<N, M> l(*this, pieces);
        Permutation<M> p = get_sub_permutation<M>(pieces);
        
        return l.index() * p.cardinality() + p.index();
    }
    
    template<long unsigned M>
    void set_from_partial_index(const unsigned &c, const std::array<unsigned, M> &pieces) {
        Permutation<M> p(c % Permutation<M>::CARD);
        Layout<N, M> l(c / Permutation<M>::CARD);

        unsigned k = 0;
        for (unsigned i = 0; i < N; ++i) {
            if (l[i] == 1) {
                (*this)[i] = pieces[p[k]];
                ++k;
            } else {
                (*this)[i] = N;
            }
        }
    }
};

template<unsigned N>
constexpr Permutation<N> operator*(const Permutation<N> &p1, const Permutation<N> &p2) {
    auto ret = p1;
    ret.compose(p2);
    return ret;
}

template<unsigned N>
bool operator==(const Permutation<N> &p1, const Permutation<N> &p2) {
    for (unsigned k = 0; k < N; ++k) {
        if (p1[k] != p2[k]) return false;
    }
    return true;
}

template<typename array_t, unsigned N>
constexpr void permute(array_t &items, const Permutation<N> &perm) {
    auto buf = items;
    for (unsigned k = 0; k < N; ++k){
        items[k] = buf[perm[k]];
    }
}

template<unsigned N, unsigned v = 2>
struct Orientation : std::array<unsigned, N> {
    // array that stores the orientations of N pieces modulo v

    constexpr Orientation() { // Constructeur sans arguments
        std::array<unsigned, N>::fill(0);
    }
    template<typename... Args>
    constexpr Orientation(Args... args) : std::array<unsigned, N>{{ static_cast<unsigned>(args)... }} {} // Constructeur par brace-enclosed

    static constexpr unsigned CARD = ipow(v, N);
    static constexpr unsigned cardinality() { // CHECKME : unnecessary ?
        return CARD;
    }

    void reset() {
        std::array<unsigned, N>::fill(0);
    }

    unsigned index(const bool even = false) const {
        // if an Orientation is even, then the total
        // orientation of the pieces is 0 modulo v.
        // In this case the orientation of the last piece
        // is forced by the others.
        static_assert(N > 0); // 0 size orientations are a no go

        unsigned n;
        if (even) { // most puzzles have a 0 net orientation
            assert(N > 1); // trivial case
            n = N - 1;
        }
        else n = N;

        unsigned ret = 0;
        unsigned base = 1;
        for (unsigned k = 0; k < n; ++k) {
            assert((*this)[k] < v);
            ret *= v;
            ret += (*this)[k];
        }
        return ret;
    }

    void set_from_index(unsigned c, const bool even = false) {
        static_assert(N > 0); // 0 size orientations are a no go

        unsigned n;
        if (even) {
            assert(N > 1); // trivial case
            assert(c < CARD / v);
            n = N - 1;
        }
        else {
            assert(c < CARD);
            n = N;
        }
        
        unsigned s = 0; // unused for non even global orientation
        for (unsigned i = n - 1; i < N; --i){
            (*this)[i] = c % v;
            s += (*this)[i];
            c = c / v;
        }
        if (even) {
            (*this)[n] = (v - (s % v)) % v;
        }
    }

    bool is_solved() const {
        for (unsigned k : *this){
            if (k != 0) return false;
        }
        return true;
    }

    void permute(const Permutation<N> &perm) {
        permute(*this, perm);
    }

    void compose(const Orientation<N, v> &ori) {
        for (unsigned k = 0; k < N; ++k){
            (*this)[k] = ((*this)[k] + ori[k]) % v;
        }
    }

    void random_state(const bool even = false) {
        srand(time(0));
        unsigned p = 0;
        for (unsigned i = 0; i < N - 1; ++i) {
            (*this)[i] = rand() % v;
            p += (*this)[i];
        }
        if (even) {
            (*this)[N - 1] = (v - p % v) % v;
        } else {
            (*this)[N - 1] = rand() % v;
        }
    }
};

template<unsigned N, unsigned v>
constexpr Orientation<N, v> operator+(const Orientation<N, v> &o1, const Orientation<N, v> &o2) {
    Orientation<N, v> ret;
    for (unsigned k = 0; k < N; ++k) {
        ret[k] = (o1[k] + o2[k]) % v;
    }
    return ret;
}

template<unsigned N, unsigned v>
constexpr Orientation<N, v> operator*(const Orientation<N, v> &o, const Permutation<N> &p) {
    Orientation<N, v> ret = o;
    permute(ret, p);
    return ret;
}

template<unsigned N, unsigned v>
bool operator==(const Orientation<N, v> &o1, const Orientation<N, v> &o2) {
    for (unsigned k = 0; k < N; ++k) {
        if (o1[k] != o2[k]) return false;
    }
    return true;
}

template<unsigned N, unsigned M>
struct Center : std::array<unsigned, N> {
    // Holds the position of one color pieces, like the center pieces
    // of an NxNxN cubic puzzle
    // N = total number of pieces
    // M = number of colors (faces)
    // M must divide N
    // NP number of equivalent pieces on each face
    // For N=12 and M=4 the
    // solved state is {0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3}

    static constexpr unsigned NP = N / M; // number of equivalent pieces on each face
    constexpr Center() {
        static_assert(M < N);
        static_assert(N % M == 0);
        reset();
    }
    template<typename... Args>
    constexpr Center(Args... args) : std::array<unsigned, N>{{ static_cast<unsigned>(args)... }} { // Constructeur par brace-enclosed
        static_assert(M < N);
        static_assert(N % M == 0);
    }

    template<unsigned K = 0>
    static constexpr unsigned cardinality() {
        if constexpr (K == M - 1) return 1;
        else {
            return binomial(N - K * NP, NP) * cardinality<K + 1>();
        }
    }
    static constexpr unsigned CARD = cardinality();

    void random_state() {
        random_shuffle(*this);
    }

    void reset() {
        for (unsigned k = 0; k < N; ++k) {
            this->operator[](k) = k / NP;
        }
    }


    bool is_solved() const {
        for (unsigned k = 0; k < N; ++k) {
            if (this->operator[](k) != k / NP) return false;
        }
        return true;
    }

    template<unsigned K = 0>
    unsigned index() const {
        if constexpr (K == M - 1) return 0;
        else {
            std::array<unsigned, N - NP * K> layout;
            unsigned k = 0;
            for (unsigned i = 0; i < N; ++i) {
                if ((*this)[i] == K) {layout[k] = 1; ++k;}
                else if ((*this)[i] > K) {layout[k] = 0; ++k;}
            }
            return layout_index(layout, NP) + binomial(N - NP * K, NP) * index<K + 1>();
        }
    }

    template<unsigned K = 0>
    void set_from_index(const unsigned c) {
        if constexpr(K == 0) this->fill(M); // fill with inconsistent value
        if constexpr(K == M - 1){
            for (unsigned i = 0; i < N; ++i){
                if ((*this)[i] >= M) {(*this)[i] = M - 1;}
            }
        } else {
            std::array<unsigned, N - K * NP> loc_layout;
            unsigned k = 0;
            layout_from_index(c % binomial(N - K * NP, NP), loc_layout, NP);
            for (unsigned i = 0; i < N; ++i) {
                if ((*this)[i] >= M) {
                    if (loc_layout[k] == 1) {
                        (*this)[i] = K;
                    }
                    ++k;
                }
            }
            assert(k == N - K * NP);
            set_from_index<K + 1>(c / binomial(N - K * NP, NP));
        }
    }
};