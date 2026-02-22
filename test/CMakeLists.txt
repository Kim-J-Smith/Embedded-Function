# The minimum CMake version
cmake_minimum_required(VERSION 3.15)

# Prevent in-source builds (force out-of-source build in ./build/)
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
  message(FATAL_ERROR "In-source builds are not allowed. Please use 'cmake -B build'.")
endif()

# Project name.
project("GoogleTest-embed_function_2.0.x" LANGUAGES C CXX)

# Include header search path
include_directories(
    ${CMAKE_SOURCE_DIR}/../include
    ${CMAKE_SOURCE_DIR}/google-test/
    ${CMAKE_SOURCE_DIR}/google-test/include
)

# Get the gtest sourece files.
file(
    GLOB GTEST_SOURCE_FILES 
    "${CMAKE_SOURCE_DIR}/google-test/src/*.cc"
)

# Library of the gtest
add_library(
    gtest_lib STATIC
    ${GTEST_SOURCE_FILES}
)

# Get the test source files.
file(
    GLOB EMBED_FUNCTION_TEST_FILES
    "${CMAKE_SOURCE_DIR}/test_*.cpp"
)

# Target executable file.
add_executable(
    ${PROJECT_NAME}
    ${EMBED_FUNCTION_TEST_FILES}
)

# Configuration of Target executable file.
set_target_properties(
    ${PROJECT_NAME}
    PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

# Link GTest library.
target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE gtest_lib
)

# Custom target to run tests and save results
add_custom_target(
    run
    COMMAND $<TARGET_FILE:${PROJECT_NAME}>
    DEPENDS ${PROJECT_NAME}
    COMMENT "Running tests..."
)

# if use MSVC, use /utf-8, use standard __cplusplus
if(MSVC)
  target_compile_options(
    ${PROJECT_NAME} PRIVATE
    /utf-8
    /Zc:__cplusplus
    /Zc:preprocessor
    /sdl
    /permissive-
    /W4
    /Wall
    /wd4710
    /wd4127
  )
elseif(CMAKE_COMPILER_IS_GNUCXX)
  target_compile_options(
    ${PROJECT_NAME} PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -Wconversion
    -Wold-style-cast
    -pedantic
    -fno-permissive
    -O2
    -fmax-errors=50
    -fno-exceptions
  )
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  target_compile_options(
    ${PROJECT_NAME} PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -Wconversion
    -Wold-style-cast
    -pedantic
    -fno-permissive
    -fno-exceptions
    -O2
  )
endif()
