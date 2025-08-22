#pragma once
#include <algorithm>   // std::fill, std::all_of
#include <filesystem>  // locate table files
#include <fstream>     // write tables into files
#include <memory>      // std::shared_ptr

#include "cubie_cube.hpp"
#include "move.hpp"

namespace fs = std::filesystem;

template <unsigned N>
struct MoveTable {
    std::shared_ptr<unsigned[]> table{new unsigned[N * N_HTM_MOVES]};

    fs::path move_table_dir() const { return "move_tables/"; }

    MoveTable() { std::fill(table.get(), table.get() + N, N); }

    template <typename Indexer, typename Unindexer>
    void compute(const Indexer &index, const Unindexer &from_index) {
        // index : a function that takes in a CubieCube instance and returns
        // some coordinate for it from_index : a function that takes in a
        // coordinate value and returns a CubieCube instance for it

        for (unsigned c = 0; c < N; ++c) {
            CubieCube cc = from_index(c);
            for (Move m : HTM_Moves) {
                auto buffer_cc = cc;
                buffer_cc.apply(m);
                table[c * N_HTM_MOVES + m] = index(buffer_cc);
            }
        }
    }

    bool is_filled() const {
        // Check if all entries are assigned
        return std::all_of(table.get(), table.get() + N * N_HTM_MOVES,
                           [](const unsigned &e) { return e < N; });
    }

    void apply(const Move &m, unsigned &c) { c = table[c * N_HTM_MOVES + m]; }

    bool load(fs::path filename) {
        auto table_path = move_table_dir() / filename;
        if (fs::exists(table_path)) {
            std::ifstream istrm(table_path, std::ios::binary);
            istrm.read(reinterpret_cast<char *>(table.get()),
                       sizeof(unsigned) * N * N_HTM_MOVES);
            istrm.close();
            assert(is_filled());
            return true;
        } else {
            print("Move table not found at: ", table_path);
        }
        return false;
    }

    void write(fs::path filename) const {
        auto table_path = move_table_dir() / filename;
        fs::create_directories(table_path.parent_path());
        std::ofstream file(table_path, std::ios::binary);
        file.write(reinterpret_cast<const char *>(table.get()),
                   sizeof(unsigned) * N * N_HTM_MOVES);
        file.close();
    }
};