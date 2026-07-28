#!/bin/bash
# Test with GCC compilers in Ubuntu

SCRIPT_CURRENT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# TEST_USE_MACROS_HELPER is defined in common.sh
source "$SCRIPT_CURRENT_DIR"/common.sh

BUILD_TYPE="Debug"
CXX_COMPILERS=("g++-13" "g++-14")
CXX_STANDARDS=("11" "14" "17" "20" "23")

environment_setup() {
    sudo apt-get update -y
    sudo apt-get install -y --no-install-recommends cmake ninja-build
    sudo apt-get install -y --no-install-recommends "${CXX_COMPILERS[@]}"
}

# $1: C++ standard version
# $2: C++ compiler
# $3: Test use macros
config_cmake_and_run_test() {
    cmake -B build -S . \
        -DCMAKE_BUILD_TYPE=${BUILD_TYPE} \
        -DCMAKE_CXX_STANDARD="$1" \
        -DCMAKE_CXX_COMPILER="$2" \
        -DCMAKE_C_COMPILER=gcc \
        -DTEST_USE_MACROS="$3"
    
    echo "============================================================"
    echo "C++ standard version: $1 | C++ compiler: $2"
    echo "============================================================"
    echo "Build type: $BUILD_TYPE"
    echo "Test use macros: $3"
    echo "============================================================"

    cmake --build build --config "$BUILD_TYPE" --target test --parallel
}

test_gcc() {
    environment_setup
    for cxx_standard in "${CXX_STANDARDS[@]}"; do
        for cxx_compiler in "${CXX_COMPILERS[@]}"; do
            for test_use_macros in "${TEST_USE_MACROS_HELPER[@]}"; do
                config_cmake_and_run_test "$cxx_standard" "$cxx_compiler" "$test_use_macros"
            done
        done
    done
}

test_gcc
