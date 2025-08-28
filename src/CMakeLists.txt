add_library(epicube-core INTERFACE)
add_library(epicube::core ALIAS epicube-core)
set_target_properties(epicube-core PROPERTIES EXPORT_NAME core)
target_sources(epicube-core INTERFACE
        coordinate.hpp
        cubie_cube.hpp
        cubie.hpp
        move.hpp
        move_table.hpp
        multistep.hpp
        node.hpp
        option.hpp
        pruning_table.hpp
        search.hpp
        utils.hpp)
target_include_directories(epicube-core INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
