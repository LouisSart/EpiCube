#include "permutation.hpp"

#include <set>
#include <cassert>

void permutation_test() {
    // Test if indexing is correct
    Permutation<6> p;
    unsigned psize = factorial(3);
    for(unsigned c = 0; c < p.cardinality(); ++c) { // Any parity
        p.set_from_index(c);
        assert(p.index() == c);

        auto [cl, p1, p2] = p.split_indices();
        unsigned e = (cl * psize + p1) * psize + p2;
        assert(e < p.cardinality());

        p.set_from_split_indices(cl, p1, p2);
        assert(p.index() == c);
    }
    Permutation<6> q;
    q.random_state(true); // even parity random
    assert(q.parity() == 0);
    q.reset();
    for(unsigned c = 0; c < q.cardinality() / 2; ++c) {
        q.set_from_index(c, true);
        assert(q.index(true) == c);

        auto [cl, p1, p2] = q.split_indices();
        q.set_from_split_indices(cl, p1, p2);
        assert(q.parity() == 0);
        assert(q.index(true) == c);
    }
    unsigned card = Layout<11, 4>::CARD * Permutation<4>::CARD;
    Permutation<11> r;
    for(unsigned c = 0; c < card; ++c) { // Any parity
        r.set_from_partial_index<4>(c, {1,3,5,7});
        assert(r.partial_index<4>({1,3,5,7}) == c);
    }
}

void orientation_test(){
    Orientation<7> o; // any orientation
    assert(o.cardinality() == Orientation<7>::CARD);
    o.random_state();
    o.reset();
    for (unsigned k = 0; k < o.cardinality(); ++k) {
        o.set_from_index(k);
        assert(o.index() == k);
    }

    Orientation<6, 2> O; // even parity
    for (unsigned k = 0; k < O.cardinality() / 2; ++k) {
        O.set_from_index(k, true);
        assert(O.index(true) == k);
    }

    // partial orientation
    unsigned card = Orientation<3, 4>::CARD;
    Layout<7, 3> l; l.random_state();
    Orientation<7, 4> po;
    Orientation<3, 4> sub_po;

    for (unsigned c = 0; c < card; ++c) {
        po.set_from_partial_index(c, l);
        assert(po.get_sub_orientation(l).index() == c);
    }
}

void layout_test() {
    Layout<10, 3> l;
    assert((l.cardinality() == Layout<10, 3>::CARD));
    l.random_state();
    l.reset();
    for (unsigned c = 0; c < l.cardinality(); ++c){
        l.set_from_index(c);
        assert(l.index() == c);
    }
}

void center_test() {
    Center<12, 4> center;
    center.random_state();
    center.reset();
    for (unsigned c = 0; c < center.cardinality(); ++c){
        center.set_from_index(c);
        assert(center.index() == c);
    }
}

void inverse_test() {
        // Tests on inverse and conjugation
    Permutation<12> aa, conj;
    aa.random_state();
    conj.random_state();
    auto i = aa.get_inverse();
    auto bb = aa.get_conjugate(conj);
    bb.compose(i.get_conjugate(conj));
    assert(bb.is_solved());
}

int main(int argc, const char* argv[]) {

    permutation_test();
    orientation_test();
    layout_test();
    center_test();
    inverse_test();

    return 0;
}