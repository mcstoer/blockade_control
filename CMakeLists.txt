cmake_minimum_required(VERSION 3.26.3 FATAL_ERROR)
project(cpp_project LANGUAGES CXX)

option(GEN_TESTS "Generate Tests" FALSE)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(Sanitizers.cmake)

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

include_directories("./include")

add_executable(main
    src/main.cpp
    include/game.hpp
    src/game.cpp
    include/components/piece.hpp
    src/components/piece.cpp
    include/components/triangle.hpp
    src/components/triangle.cpp
    include/components/square.hpp
    src/components/square.cpp
    include/components/rectangle.hpp
    src/components/rectangle.cpp
    include/components/board.hpp
    src/components/board.cpp
    include/input/input_handler.hpp
    src/input/input_handler.cpp
    include/input/actions.hpp
    include/actors/player.hpp
    src/actors/player.cpp
)

target_link_libraries(main OpenGL::GL glfw)

if (GEN_TESTS)
    # Board tests
    add_executable(test_board
        tests/test_board.cpp
        include/components/piece.hpp
        src/components/piece.cpp
        include/components/triangle.hpp
        src/components/triangle.cpp
        include/components/square.hpp
        src/components/square.cpp
        include/components/rectangle.hpp
        src/components/rectangle.cpp
        include/components/board.hpp
        src/components/board.cpp
    )
        

endif()
