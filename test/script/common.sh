#!/bin/bash
# Common functions for test scripts

set -e

TEST_USE_MACROS_ARRAY=(
    "EBD_TEST_USE_FALLBACK"
)

TEST_USE_MACROS_HELPER=(
    ""
    "${TEST_USE_MACROS_ARRAY[0]}"
)
