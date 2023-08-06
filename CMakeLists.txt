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

add_executable(game 
    src/game.cpp
    include/components/piece.hpp
    src/components/piece.cpp
    include/components/triangle.hpp
    src/components/triangle.cpp
    include/components/square.hpp
    src/components/square.cpp
    include/components/rectangle.hpp
    src/components/rectangle.cpp
    include/board.hpp
    src/board.cpp
)

target_link_libraries(game OpenGL::GL glfw)

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
        include/board.hpp
        src/board.cpp
    )
        

endif()
