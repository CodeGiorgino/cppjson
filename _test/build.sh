#!/usr/bin/env bash
set -e

function do_build() {
    # ensure targets
    [ -d build ] && rm -rf build
    mkdir build

    # compilation
    for file in src/*.cpp; do
        filename="$(basename $file)"
        filename="${filename%.*}"
        echo "[?] info: building file: $file"
        g++ -Wall -Wextra -std=c++23 -$OPT_LEVEL -o build/$filename $file ../build/json.a
    done
}

function do_clean() {
    # remove targets
    [ -d build ] && rm -rf build
}

# check args
if [ $# -eq 0 ]; then
    OPT_LEVEL=O2
    echo '[?] info: DEBUG set to false'
    do_build
elif [ $# -gt 1 ]; then
    echo '[!] error: too many arguments provided'
    exit 1
else
    case $1 in
        -d | --debug)
            OPT_LEVEL=ggdb
            echo '[?] info: DEBUG set to true'
            do_build
            ;;

        --clean)
            echo '[?] info: cleaning the targets'
            do_clean
            ;;

        *)
            echo "[!] error: unknown option provided: $1"
            exit 1
    esac
fi



