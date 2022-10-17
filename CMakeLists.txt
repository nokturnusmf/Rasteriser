cmake_minimum_required(VERSION 3.11)

project(Rasterizer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_EXPORT_COMPILE_COMMANDS True)

set(CMAKE_INTERPROCEDURAL_OPTIMIZATION True)

add_compile_options(-Wall -Wextra)

add_subdirectory(src)
