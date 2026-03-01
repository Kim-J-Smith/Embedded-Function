# The minimum CMake version
cmake_minimum_required(VERSION 3.15)

# Prevent in-source builds (force out-of-source build in ./build/)
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
  message(FATAL_ERROR "In-source builds are not allowed. Please use 'cmake -B build'.")
endif()

# Project name.
project(
    "embed_function_2.0.x-GoogleTest" 
    VERSION 2.0.2 
    LANGUAGES CXX
)

# export command
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_EXTENSIONS OFF)
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  add_compile_definitions(
    _CRT_NONSTDC_NO_DEPRECATE
    _CRT_SECURE_NO_WARNINGS
    _BSD_SOURCE
  )
endif()

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

# Configuration of GTest file.
set_target_properties(
    gtest_lib
    PROPERTIES
    CXX_STANDARD 11
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

# Link GTest library.
target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE gtest_lib
)

# Custom target to run tests and save results
add_custom_target(
    test
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
    /wd4514
    /wd4626
    /wd5045
  )
  target_compile_options(
    gtest_lib INTERFACE
    /w
    /wd4711
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
    -Os
    -fmax-errors=50
    -fno-exceptions
  )
  target_compile_options(
    gtest_lib PRIVATE
    -O1
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
    -Os
  )
  target_compile_options(
    gtest_lib PRIVATE
    -O1
  )
endif()
