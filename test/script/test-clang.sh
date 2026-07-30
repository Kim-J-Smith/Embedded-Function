#!/bin/bash
# Test with Clang compilers in Ubuntu

SCRIPT_CURRENT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# TEST_USE_MACROS_HELPER is defined in common.sh
source "$SCRIPT_CURRENT_DIR"/common.sh

BUILD_TYPE="Debug"
COMPILER_VERSIONS=("$@")
CXX_STANDARDS=("11" "14" "17" "20" "23")

environment_setup() {
    sudo apt-get update -y
    sudo apt-get install -y --no-install-recommends cmake ninja-build
    sudo apt-get install -y --no-install-recommends "${COMPILER_VERSIONS[@]/#/clang-}"
}

# $1: C++ standard version
# $2: C/C++ compiler version
# $3: Test use macros
config_cmake_and_run_test() {
    echo; echo; echo;
    echo "============================================================"
    echo "C++ standard version: $1 | C++ compiler: clang++ $2"
    echo "============================================================"
    echo "Build type: $BUILD_TYPE"
    echo "Test use macros: $3"
    echo "============================================================"

    cmake -B build -S . -G Ninja \
        -DCMAKE_BUILD_TYPE=${BUILD_TYPE} \
        -DCMAKE_CXX_STANDARD="$1" \
        -DCMAKE_CXX_COMPILER="clang++-$2" \
        -DCMAKE_C_COMPILER="clang-$2" \
        -DTEST_USE_MACROS="$3"
    cmake --build build --config "$BUILD_TYPE" --target test --parallel
}

test_clang() {
    environment_setup
    echo; echo "-- FINISH ENVIRONMENT SETUP --"; echo;
    for compiler_version in "${COMPILER_VERSIONS[@]}"; do
        for cxx_standard in "${CXX_STANDARDS[@]}"; do
            for test_use_macros in "${TEST_USE_MACROS_HELPER[@]}"; do
                config_cmake_and_run_test "$cxx_standard" "$compiler_version" "$test_use_macros"
            done
        done
    done
}

test_clang
