#pragma once
#include <iostream>
#include <memory>  // shared_ptr
#include <vector>  // expand() return type

#include "algorithm.hpp"  // last moves

template <typename Cube>
struct Node : public std::enable_shared_from_this<Node<Cube>> {
    using sptr = std::shared_ptr<Node<Cube>>;
    using csptr = std::shared_ptr<Node<Cube> const>;

    Cube state;      // The cube state this node corresponds to
    sptr parent;     // The shared_ptr to the parent
    unsigned depth;  // The number of moves made to get this state
    Move last_move;  // The moves which yielded this state

    Node() : state{Cube()}, depth{0}, parent{nullptr} {}
    Node(const Cube &c, const unsigned &d = 0, sptr p = nullptr,
         const Move &move = D)
        : state{c}, depth{d}, parent{p}, last_move{move} {}

   public:
    bool is_root() const { return parent == nullptr; }

    template <typename F, typename MoveContainer>
    std::vector<sptr> expand(const F &apply, const MoveContainer &directions) {
        // Generates the children of the current node
        std::vector<sptr> children;
        Cube next;
        for (auto &&move : directions) {
            next = state;
            apply(move, next);
            children.emplace_back(
                new Node(next, depth + 1, this->shared_from_this(), move));
        }
        return children;
    };

    Algorithm get_path(const bool inverse = false) const {
        Algorithm path;
        path.inv_flag = inverse;
        csptr p = this->shared_from_this();
        while (p->parent != nullptr) {
            path.prepend(p->last_move);
            p = p->parent;
        }
        return path;
    }

    void show() const {
        std::cout << "Node object: " << std::endl;
        std::cout << " Depth: " << depth << std::endl;
        std::cout << " Cube type: " << typeid(state).name() << std::endl;
    }
};

template <typename Cube>
typename Node<Cube>::sptr make_root(const Cube &cube) {
    return typename Node<Cube>::sptr(new Node(cube, 0));
}

template <typename Cube>
typename Node<Cube>::sptr make_node(const Cube &cube, const unsigned &depth) {
    return typename Node<Cube>::sptr(new Node(cube, depth));
}
