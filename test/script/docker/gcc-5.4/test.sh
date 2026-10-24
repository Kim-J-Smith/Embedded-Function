#!/bin/bash
# Test with GCC in Linux

set -e

# Death tests terminate the test process on purpose; do not leave core files behind.
ulimit -c 0

SCRIPT_CURRENT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPOSITORY_ROOT="$(cd "$SCRIPT_CURRENT_DIR/../../../.." && pwd)"

cd "$REPOSITORY_ROOT/test"

c++ --version
cmake --version

run_test() {
    local test_use_macros="$1"

    echo "============================================================"
    echo "C++ standard version: 11 | C++ compiler: g++"
    echo "============================================================"
    echo "Build type: Debug"
    echo "Test use macros: [$test_use_macros]"
    echo "AddressSanitizer: disabled"
    echo "============================================================"

    rm -rf ./build
    cmake -B build -S . \
        -DCMAKE_BUILD_TYPE=Debug \
        -DCMAKE_CXX_STANDARD=11 \
        -DTEST_DISABLE_ASAN=ON \
        -DTEST_USE_MACROS="$test_use_macros"
    cmake --build build --config Debug --target test --parallel
}

run_test ""
run_test "EBD_TEST_USE_FALLBACK"
