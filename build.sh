#!/usr/bin/env bash
set -e

function do_build() {
    # ensure targets
    [ -d obj ] && rm -rf obj
    [ -d build ] && rm -rf build

    mkdir obj
    mkdir build
    mkdir build/include

    # compilation
    # g++ -Wall -Wextra -std=c++23 -c -$OPT_LEVEL -o obj/cppjson.o src/cppjson.cpp
    g++ -Wall -Wextra -std=c++23 -c -$OPT_LEVEL -o obj/parser.o src/parser.cpp

    # linking
    ar rs -o build/cppjson.a -- obj/cppjson.o obj/parser.o
    # cp src/cppjson.hpp src/parser.hpp -- build/include/
}

function do_clean() {
    # remove targets
    [ -d obj ] && rm -rf obj
    [ -d build ] && rm -rf build
}

# check args
if [ $# -eq 0 ]; then
    OPT_LEVEL=O2
    echo 'info: DEBUG set to false'
    do_build
elif [ $# -gt 1 ]; then
    echo 'error: too many arguments provided'
    exit 1
else
    case $1 in
        -d | --debug)
            OPT_LEVEL=g
            echo 'info: DEBUG set to true'
            do_build
            ;;

        --clean)
            echo 'info: cleaning the targets'
            do_clean
            ;;

        *)
            echo "error: unknown option provided: $1"
            exit 1
    esac
fi



