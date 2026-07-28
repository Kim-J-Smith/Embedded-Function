#!/bin/bash
# Test with MSVC compilers in Windows

SCRIPT_CURRENT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# TEST_USE_MACROS_HELPER is defined in common.sh
source "$SCRIPT_CURRENT_DIR"/common.sh

BUILD_TYPE="Debug"
COMPILER_MSVC_G=("$@")
CXX_STANDARDS=("14" "17" "20" "23")

# $1: C++ standard version
# $2: C/C++ compiler generator (-G)
# $3: Test use macros
config_cmake_and_run_test() {
    echo; echo; echo;
    echo "============================================================"
    echo "C++ standard version: $1 | C++ compiler: $2"
    echo "============================================================"
    echo "Build type: $BUILD_TYPE"
    echo "Test use macros: $3"
    echo "============================================================"

    cmake -B build -S . -G "$2" \
        -DCMAKE_BUILD_TYPE=${BUILD_TYPE} \
        -DCMAKE_CXX_STANDARD="$1" \
        -DTEST_USE_MACROS="$3"
    cmake --build build --config "$BUILD_TYPE" --target test --parallel
}

test_msvc() {
    for compiler_generator in "${COMPILER_MSVC_G[@]}"; do
        for cxx_standard in "${CXX_STANDARDS[@]}"; do
            for test_use_macros in "${TEST_USE_MACROS_HELPER[@]}"; do
                config_cmake_and_run_test "$cxx_standard" "$compiler_generator" "$test_use_macros"
            done
        done
    done
}

test_msvc
