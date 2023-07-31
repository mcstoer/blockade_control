cmake_minimum_required(VERSION 3.26.3 FATAL_ERROR)
project(cpp_project LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

add_executable(game src/game.cpp)
target_link_libraries(game OpenGL::GL glfw)
