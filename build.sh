#!/usr/bin/env bash
set -e

# ensure targets
mkdir -p obj
mkdir -p build
mkdir -p build/include

# compilation
g++ -Wall -Wextra -std=c++23 -c -o obj/cppjson.o src/cppjson.cpp
g++ -Wall -Wextra -std=c++23 -c -o obj/parser.o src/parser.cpp

# linking
ar rvs -o build/cppjson.a -- obj/cppjson.o obj/parser.o
cp src/cppjson.hpp src/parser.hpp -- build/include/
