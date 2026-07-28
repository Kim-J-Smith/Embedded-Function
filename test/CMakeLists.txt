# The minimum CMake version
cmake_minimum_required(VERSION 3.15)

# Prevent in-source builds (force out-of-source build in ./build/)
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
  message(FATAL_ERROR "In-source builds are not allowed. Please use 'cmake -B build'.")
endif()

# Project name.
project(
    "embed_function_2.0.x-GoogleTest"
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


# test-use macros
set(
  TEST_USE_MACROS ${TEST_USE_MACROS}
  # "EMBED_FN_CONFIG_USE_BIG_DEFAULT_BUFFER"
  # "EMBED_FN_CONFIG_DISABLE_SMART_FORWARD"
  # "EMBED_FN_CONFIG_UNDEF_MACROS"
  # "EMBED_FN_HOOK_DEBUG(msg)"
  # "EBD_TEST_USE_FALLBACK"
  # "EBD_TEST_TRY_BUG__GCC_106067"
  # "EBD_TEST_TRY_BUG__Clang_SameNameStaticFunction" # Clang20 - C++23/26; Fixed in Clang22
  # "EBD_TEST_TRY_BUG__MSVC_1944_1" # MSVC 19.44; Fixed in MSVC 19.50
)

# Include header search path
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
    ${CMAKE_CURRENT_SOURCE_DIR}/google-test/
    ${CMAKE_CURRENT_SOURCE_DIR}/google-test/include
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Get the gtest sourece files.
file(
    GLOB GTEST_SOURCE_FILES 
    "${CMAKE_CURRENT_SOURCE_DIR}/google-test/src/*.cc"
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
    "${CMAKE_CURRENT_SOURCE_DIR}/test_*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/conformance/fn_ref/*.pass.cpp"
)

# Target executable file.
add_executable(
    ${PROJECT_NAME}
    ${EMBED_FUNCTION_TEST_FILES}
)

# macro definitions
target_compile_definitions(
    ${PROJECT_NAME} PRIVATE
    ${TEST_USE_MACROS}
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
    /wd6326
    # /analyze 
    /WX
  )
  target_link_options(
    ${PROJECT_NAME} PRIVATE
    /OPT:NOICF
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
    -O0
    -fmax-errors=50
    -fno-exceptions
    -Werror
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
    -O0
    -Werror
  )
  target_compile_options(
    gtest_lib PRIVATE
    -O1
  )
endif()

# Fail tests
file(
  GLOB EMBED_FUNCTION_FAIL_TEST_FILES
  "${CMAKE_CURRENT_SOURCE_DIR}/conformance/fn_ref/fail/*.fail.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/conformance/fn/fail/*.fail.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/conformance/safe_fn/fail/*.fail.cpp"
)

get_property(PATH_INCLUDES DIRECTORY PROPERTY INCLUDE_DIRECTORIES)

set(FAIL_TEST_CMAKE_FLAGS "")

set(FAIL_TEST_CMAKE_FLAGS "${FAIL_TEST_CMAKE_FLAGS} -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}")
set(FAIL_TEST_CMAKE_FLAGS "${FAIL_TEST_CMAKE_FLAGS} -DCMAKE_CXX_STANDARD=${CMAKE_CXX_STANDARD}")

set(FAIL_TEST_INCLUDE_PATH_CMD "")
foreach(inc_directory ${PATH_INCLUDES})
  if(MSVC)
    set(FAIL_TEST_INCLUDE_PATH_CMD "${FAIL_TEST_INCLUDE_PATH_CMD} /I${inc_directory}")
  else()
    set(FAIL_TEST_INCLUDE_PATH_CMD "${FAIL_TEST_INCLUDE_PATH_CMD} -I${inc_directory}")
  endif()
endforeach()

foreach(macros_ ${TEST_USE_MACROS})
  if(MSVC)
    set(FAIL_TEST_INCLUDE_PATH_CMD "${FAIL_TEST_INCLUDE_PATH_CMD} /D${macros_}")
  else()
    set(FAIL_TEST_INCLUDE_PATH_CMD "${FAIL_TEST_INCLUDE_PATH_CMD} -D${macros_}")
  endif()
endforeach()

if(MSVC)
  set(FAIL_TEST_INCLUDE_PATH_CMD "${FAIL_TEST_INCLUDE_PATH_CMD} /analyze /WX")
else()
  set(FAIL_TEST_INCLUDE_PATH_CMD "${FAIL_TEST_INCLUDE_PATH_CMD} -Werror")
endif()

message(STATUS "===---------------------- FAIL TEST BEGIN ----------------------===")
message(STATUS "Flags: ${FAIL_TEST_CMAKE_FLAGS}")

foreach(FAIL_TEST_SRC ${EMBED_FUNCTION_FAIL_TEST_FILES})
  get_filename_component(FAIL_TEST_NAME ${FAIL_TEST_SRC} NAME_WE)
  get_filename_component(FAIL_TEST_FILE ${FAIL_TEST_SRC} NAME)

  try_compile(
    COMPILE_RESULT
    ${CMAKE_CURRENT_BINARY_DIR}/fail_test/${FAIL_TEST_NAME}
    SOURCES ${FAIL_TEST_SRC}
    CMAKE_FLAGS "${FAIL_TEST_CMAKE_FLAGS}"
    COMPILE_DEFINITIONS "${CMAKE_CXX_FLAGS} ${FAIL_TEST_INCLUDE_PATH_CMD}"
  )

  if(${COMPILE_RESULT})
    message(
      FATAL_ERROR
      "[  ERROR   ]: ${FAIL_TEST_FILE} passed but it should be rejected!"
      " See ${FAIL_TEST_SRC}"
    )
  else()
    message(
      STATUS
      "[  PASSED  ]: ${FAIL_TEST_FILE} correctly rejected"
    )
  endif()
endforeach()

message(STATUS "===----------------------- FAIL TEST END -----------------------===")
