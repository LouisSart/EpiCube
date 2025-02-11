#pragma once
#include <algorithm>  // std::reverse
#include <cassert>    // assert
#include <iostream>   // std::cout
#include <sstream>    // std::stringstream
#include <vector>     // Move sequence

#include "move.hpp"

struct Algorithm {
    std::vector<Move> sequence;
    bool inv_flag;

    explicit Algorithm(const bool &flag = false) : inv_flag{flag} {};
    Algorithm(std::initializer_list<Move> &&s, const bool &flag = false)
        : sequence{s}, inv_flag{flag} {};
    Algorithm(char const *repr, const bool &flag = false) : inv_flag{flag} {
        // A lot of types can be casted to std::string
        // so sometimes overload resolution picks this constructor
        std::string str{repr};
        if (str[0] == '(') {  // Turn on inverse flag if alg starts with '('
            assert(str.back() == ')');
            inv_flag = true;
            str = std::string(str.begin() + 1, str.end() - 1);
        }

        std::stringstream s(str);
        std::string move_str;
        while (s >> move_str) {
            // Run through the string and add the moves to the sequence
            if (str_to_move.find(move_str) != str_to_move.end()) {
                append(str_to_move[move_str]);
            } else {
                // Raise error if character is no a move
                // Missing blank space between moves
                // will result in an error
                std::cout << "Error reading scramble" << std::endl;
                abort();
            }
        }
    }

    void append(const Algorithm &other) {
        sequence.insert(sequence.end(), other.sequence.begin(),
                        other.sequence.end());
    }

    void prepend(const Algorithm &other) {
        sequence.insert(sequence.begin(), other.sequence.begin(),
                        other.sequence.end());
    }

    Algorithm reversed() const {
        auto ret = *this;
        std::reverse(ret.sequence.begin(), ret.sequence.end());
        return ret;
    }

    void append(const Move &move) { sequence.push_back(move); }

    Move back() { return sequence.back(); }

    auto size() const { return sequence.size(); }

    void operator<<(std::ostream &os) const;

    Algorithm get_inverse() const;
    void show() const;
};

std::ostream &operator<<(std::ostream &os, const Algorithm &alg) {
    if (alg.inv_flag) os << "(";
    for (auto move : alg.sequence) {
        os << move << " ";
    }
    if (alg.inv_flag) os << "\b) ";
    return os;
}

void Algorithm::show() const {
    std::cout << *this;
    if (sequence.size() > 0) std::cout << "(" << size() << ")" << std::endl;
}

struct StepAlgorithm : Algorithm {
    std::string comment;

    StepAlgorithm() {}
    // explicit StepAlgorithm(std::string str, std::string c = "")
    //     : Algorithm{str}, comment{c} {}
    explicit StepAlgorithm(const Algorithm &alg, std::string c = "")
        : Algorithm{alg}, comment{c} {}

    void show(unsigned previous_moves = 0) const {
        std::cout << *this << "// " << comment << " (" << size() << "/"
                  << size() + previous_moves << ")" << std::endl;
    }
};

struct Skeleton : std::vector<StepAlgorithm> {
    void show() const {
        unsigned moves_made = 0;
        for (auto step : *this) {
            step.show(moves_made);
            moves_made += step.size();
        }
    }
};

Algorithm Algorithm::get_inverse() const {
    auto ret = *this;
    std::reverse(ret.sequence.begin(), ret.sequence.end());
    for (auto &m : ret.sequence) {
        m = inverse_of_HTM_Moves[m];
    }
    return ret;
}