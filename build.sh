#!/usr/bin/env bash
set -e

# ensure targets
rm -rf obj
rm -rf build

mkdir obj
mkdir build
mkdir build/include

# compilation
g++ -Wall -Wextra -std=c++23 -c -O2 -o obj/cppjson.o src/cppjson.cpp
g++ -Wall -Wextra -std=c++23 -c -O2 -o obj/parser.o src/parser.cpp

# linking
ar rvs -o build/cppjson.a -- obj/cppjson.o obj/parser.o
cp src/cppjson.hpp src/parser.hpp -- build/include/
