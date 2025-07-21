#pragma once
#include <algorithm>  // std::fill(begin, end)
#include <cstdint>    // uint8_t
#include <deque>      // std::deque
#include <filesystem> // locate table files
#include <fstream>    // write tables into files
#include <limits>     // std::numeric_limits
#include <memory>     // std::shared_ptr

#include "coordinate.hpp"
#include "move.hpp"
#include "utils.hpp"

namespace fs = std::filesystem;

template <std::size_t N> struct PruningTable {
  using entry_type = uint8_t; // Cannot be printed, max representable
  // value of uint8_t is 255
  static constexpr unsigned unassigned = std::numeric_limits<entry_type>::max();

  std::shared_ptr<entry_type[]> table{new entry_type[N]};

  PruningTable() { std::fill(table.get(), table.get() + N, unassigned); }

  unsigned estimate(const unsigned &index) const {
    assert(index < N);
    return table[index];
  }

  void set(const unsigned &index, const unsigned &value) {
    assert(index < N);
    assert(value < unassigned); // Ensure value fits in uint8_t
    table[index] = static_cast<entry_type>(value);
  }

  void reset() { std::fill(table.get(), table.get() + N, unassigned); }

  auto get_estimator() const {
    return [this](const unsigned &index) { return this->estimate(index); };
  }

  unsigned size() const { return N; }

  entry_type &operator[](const unsigned k) const {
    assert(k < N);
    return table[k];
  }

  bool is_assigned(const unsigned &index) const {
    assert(index < N);
    return table[index] != unassigned;
  }

  bool is_filled() const {
    // Check if all entries are assigned
    return std::all_of(table.get(), table.get() + N,
                       [](const entry_type &e) { return e != unassigned; });
  }

  template <bool verbose = false, typename Cube, typename Mover,
            typename Indexer, std::size_t NM = 18>
  auto generate(const Cube &root, const Mover &apply, const Indexer &index,
                const std::array<Move, NM> &moves = HTM_Moves) {
    std::deque<Cube> queue{root};

    table[index(root)] = 0;
    unsigned i = 0, depth = 0, count = 1;

    while (!queue.empty()) {
      Cube cc = queue.back();
      i = index(cc);

      if constexpr (verbose) {
        if (table[i] > depth) {
          print(depth, count, (double)count / size() * 100, "%");
        }
      }

      depth = table[i];
      assert(i < N);

      for (auto move : moves) {
        Cube cc2 = cc;
        apply(move, cc2);

        unsigned ii = index(cc2);
        assert(ii < N);
        if (!is_assigned(ii)) {
          table[ii] = depth + 1;
          queue.push_front(cc2);
          ++count;
        }
      }
      queue.pop_back();
    }
    assert(is_filled());
    assert(count == N);
  }

  std::vector<unsigned> get_distribution() const {
    std::vector<unsigned> ret = {0};
    unsigned h;
    for (auto it = table.get(); it < table.get() + N; ++it) {
      h = (unsigned)(*it);
      if (h + 1 > ret.size()) {
        ret.resize(h + 1);
      }
      ++ret[h];
    }
    return ret;
  }

  void show_distribution() {
    auto distr = get_distribution();
    float mean = 0.0;

    for (unsigned k = 0; k < distr.size(); ++k) {
      std::cout << std::setw(2) << k << " " << distr[k] << std::endl;
      mean += k * distr[k];
    }
    std::cout << "Mean value: " << mean / N << std::endl;
  }

  fs::path pruning_table_dir() const { return "pruning_tables/"; }

  bool load(fs::path filename) {
    auto table_path = pruning_table_dir() / filename;
    if (fs::exists(table_path)) {
      std::ifstream istrm(table_path, std::ios::binary);
      istrm.read(reinterpret_cast<char *>(table.get()), sizeof(entry_type) * N);
      istrm.close();
      assert(is_filled());
      return true;
    } else {
      print("Pruning table not found at: ", table_path);
    }
    return false;
  }

  void write(fs::path filename) const {
    auto table_path = pruning_table_dir() / filename;
    fs::create_directories(table_path.parent_path());
    std::ofstream file(table_path, std::ios::binary);
    file.write(reinterpret_cast<const char *>(table.get()),
               sizeof(entry_type) * N);
    file.close();
  }
};