#!/bin/bash

set -e

mkdir -p build
cd build

cmake .. -D CMAKE_BUILD_TYPE=Release -D CMAKE_CXX_COMPILER=clang++
make
