cmake_minimum_required(VERSION 3.14)

project(embedded_function VERSION 2.0.13 LANGUAGES CXX)
add_library(embedded_function INTERFACE)
add_library(embedded_function::functions ALIAS embedded_function)
set_target_properties(embedded_function PROPERTIES EXPORT_NAME functions)

include(GNUInstallDirs)

target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_11)
target_include_directories(${PROJECT_NAME} INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

# Copy include/embed/embed_function.hpp to ${CMAKE_INSTALL_PREFIX}/include
install(
    TARGETS embedded_function
    EXPORT embedded_functionTargets
)
install(
    FILES include/embed/embed_function.hpp
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/embed
)

# Generate embedded_functionTargets.cmake
install(
    EXPORT embedded_functionTargets
    NAMESPACE embedded_function::
    DESTINATION ${CMAKE_INSTALL_DATADIR}/embedded_function
)
export(
  TARGETS embedded_function
  NAMESPACE embedded_function::
  FILE embedded_functionTargets.cmake
)

# Generate embedded_functionConfig.cmake
include(CMakePackageConfigHelpers)
configure_package_config_file(
  cmake/embedded_function.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/cmake/embedded_functionConfig.cmake
  INSTALL_DESTINATION ${CMAKE_INSTALL_DATADIR}/embedded_function
  PATH_VARS CMAKE_INSTALL_INCLUDEDIR
)

# Generate embedded_functionConfigVersion.cmake
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
  cmake/embedded_functionConfigVersion.cmake
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion
  ARCH_INDEPENDENT
)

# Install embedded_functionConfig.cmake and embedded_functionConfigVersion.cmake
install(
  FILES 
    ${CMAKE_CURRENT_BINARY_DIR}/cmake/embedded_functionConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/cmake/embedded_functionConfigVersion.cmake
  DESTINATION ${CMAKE_INSTALL_DATADIR}/embedded_function
)
