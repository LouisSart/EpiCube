#pragma once
#include <algorithm> // std::reverse
#include <iostream>  // std::cout
#include <sstream>   // std::stringstream
#include <vector>    // Move sequence

#include "move.hpp"

struct Algorithm {
  std::vector<Move> sequence;
  bool inv_flag;

  Algorithm(const bool is_on_inverse = false) : inv_flag{is_on_inverse} {};
  // Algorithm(const std::initializer_list<Move> s,
  //           const bool is_on_inverse = false)
  //     : sequence{s}, inv_flag{is_on_inverse} {};
  // Algorithm(const std::vector<Move> &s, const bool is_on_inverse = false)
  //     : sequence{s}, inv_flag{is_on_inverse} {};
  Algorithm(const std::string &str, const bool is_on_inverse = false)
      : inv_flag{is_on_inverse} {
    *this = Algorithm::make_from_str(str);
  };

  static Algorithm make_from_str(const std::string &str) {
    Algorithm ret;
    std::stringstream s(str);
    std::string move_str;
    s >> move_str;
    while (s) {
      std::cout << move_str << std::endl;
      if (str_to_move.find(move_str) != str_to_move.end()) {
        ret.append(str_to_move[move_str]);
      } else {
        std::cout << "Error reading scramble" << std::endl;
        abort();
      }
      s >> move_str;
    }
    return ret;
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
  if (alg.inv_flag)
    os << "(";
  for (auto move : alg.sequence) {
    os << move << " ";
  }
  if (alg.inv_flag)
    os << "\b)";
  return os;
}

void Algorithm::show() const {
  std::cout << *this;
  if (sequence.size() > 0)
    std::cout << "(" << size() << ")" << std::endl;
}

struct StepAlgorithm : Algorithm {
  std::string comment;

  StepAlgorithm() {}
  StepAlgorithm(Algorithm m, std::string c) : Algorithm{m}, comment{c} {}

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