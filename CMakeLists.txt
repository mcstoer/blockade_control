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

include_directories(include)

add_executable(blockadecontrol
    src/main.cpp
    src/game.cpp
    src/components/piece.cpp
    src/components/triangle.cpp
    src/components/square.cpp
    src/components/rectangle.cpp
    src/components/board.cpp
    src/input/input_handler.cpp
    src/actors/player.cpp
)

target_link_libraries(blockadecontrol OpenGL::GL glfw)

# Install the game program
INSTALL(TARGETS blockadecontrol DESTINATION bin)

# Install the demo script
INSTALL(PROGRAMS demo DESTINATION bin)

if (GEN_TESTS)
    # Board tests
    add_executable(test_board
        tests/test_board.cpp
        src/components/piece.cpp
        src/components/triangle.cpp
        src/components/square.cpp
        src/components/rectangle.cpp
        src/components/board.cpp
    )

endif()
