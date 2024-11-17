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
    for file in src/*.cpp; do
        filename="$(basename $file)"
        filename="${filename%.*}"
        echo "[?] info: building file: $file"
        g++ -Wall -Wextra -std=c++23 -c -$OPT_LEVEL -o obj/$filename.o $file
    done

    # linking
    echo "[?] info: linking object files"
    objs=(obj/*.o)
    ar rs -o build/json.a -- ${objs[@]}

    hpps=(./src/*.hpp)
    cp ${hpps[@]} -- build/include
}

function do_tests() {
    if [ ! -d _test ]; then
        echo '[!] fatal: cannot find folder `_test`'
        exit 1
    fi

    if [ ! -f _test/build.sh ]; then
        echo '[!] fatal: cannot find script file `_test/build.sh`'
        exit 1
    fi

    # build tests
    cd _test
    ./build.sh

    # run tests
    for file in build/*; do
        $file
    done
    cd -
}

function do_clean() {
    # remove targets
    [ -d obj ] && rm -rf obj
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
            echo '[?] info: building source'
            do_build
            ;;

        --clean)
            echo '[?] info: cleaning the targets'
            do_clean
            ;;

        --test)
            OPT_LEVEL=O2
            echo '[?] info: DEBUG set to false'
            echo '[?] info: building source'
            do_build
            echo '[?] info: building tests'
            echo '--------------------'
            do_tests
            ;;

        *)
            echo "[!] error: unknown option provided: $1"
            exit 1
    esac
fi



