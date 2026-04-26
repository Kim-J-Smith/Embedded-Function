# The minimum CMake version
cmake_minimum_required(VERSION 3.15)

# Prevent in-source builds (force out-of-source build in ./build/)
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
  message(FATAL_ERROR "In-source builds are not allowed. Please use 'cmake -B build'.")
endif()

# Project name.
project(
    "embed_function_2.x-Benchmark"
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
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
)

# Get the test source files.
file(
    GLOB EMBED_FUNCTION_BENCHMARK_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/runtime/benchmark_*.cpp"
)

# Target executable file.
add_executable(
    ${PROJECT_NAME}
    ${EMBED_FUNCTION_BENCHMARK_FILES}
)

# Custom target to run benchmark
add_custom_target(
    benchmark
    COMMAND $<TARGET_FILE:${PROJECT_NAME}> --out-fmt=txt 
      --output=${CMAKE_CURRENT_SOURCE_DIR}/build/runtime-report.md
    DEPENDS ${PROJECT_NAME}
    COMMENT "Running benchmark..."
    COMMENT "The result is in ${CMAKE_CURRENT_SOURCE_DIR}/build/runtime-report.md"
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
    /w
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
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  target_compile_options(
    ${PROJECT_NAME} PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -Wconversion
    -Wold-style-cast
    -Wno-sign-conversion
    -pedantic
    -fno-permissive
    -fno-exceptions
    -Os
  )
endif()
