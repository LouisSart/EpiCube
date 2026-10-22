#include "utils.hpp"
#include "cubie_cube.hpp"
#include "move.hpp"

namespace Cube222 {
    std::ostream &operator<<(std::ostream &os, const Move &m) {
        switch (m) {
            case U:  os << "U";  break;
            case U2: os << "U2"; break;
            case U3: os << "U'"; break;
            case R:  os << "R";  break;
            case R2: os << "R2"; break;
            case R3: os << "R'"; break;
            case F:  os << "F";  break;
            case F2: os << "F2"; break;
            case F3: os << "F'"; break;
            default: os << "";
        }
        return os;
    }

    void CubieCube::apply(const Move & m) {print("coucou");};
    void CubieCube::index() const {};
};