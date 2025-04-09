# SPDX-FileCopyrightText: 2025 Thomas Mathys
# SPDX-License-Identifier: MIT

set(
  argpppp_sources
  PUBLIC FILE_SET CXX_MODULES FILES
  argpppp.cppm
  of.cppm
  option.cppm
  optional_string.cppm
  parser.cppm
  pf.cppm
  PRIVATE
  option.cpp
  parser.cpp)

# Production version of argpppp
add_library(argpppp)
target_sources(argpppp ${argpppp_sources})
target_compile_definitions(argpppp PRIVATE ARGPPPP_EXPORT_FOR_UNIT_TESTING=)
target_compile_features(argpppp PUBLIC cxx_std_23)

# Unit testing version of argpppp
if(BUILD_TESTING)
  add_library(argpppp_unit_testing)
  target_sources(argpppp_unit_testing ${argpppp_sources})
  target_compile_definitions(argpppp_unit_testing PRIVATE ARGPPPP_EXPORT_FOR_UNIT_TESTING=export)
  target_compile_features(argpppp_unit_testing PUBLIC cxx_std_23)
endif()

# Check for argp_parse
include(CheckSymbolExists)
check_symbol_exists(argp_parse "argp.h" ARGPPPP_HAVE_ARGP)

# Use argp-standalone if argp_parse is not around
if(NOT ARGPPPP_HAVE_ARGP)
  include(FetchContent)
  FetchContent_Declare(
    argp-standalone
    GIT_REPOSITORY https://github.com/tom42/argp-standalone
    GIT_TAG        v1.0.3)
  FetchContent_MakeAvailable(argp-standalone)
  target_link_libraries(argpppp PRIVATE argp-standalone)
  target_link_libraries(argpppp_unit_testing PRIVATE argp-standalone)
endif()

# Install argpppp
# Based on https://crascit.com/2024/04/04/cxx-modules-cmake-shared-libraries/#h-installing-shared-libraries-with-c-20-modules
include(GNUInstallDirs)
install(
  TARGETS argpppp
  EXPORT argpppp-targets
  FILE_SET CXX_MODULES
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/argpppp/src
  FILE_SET HEADERS
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  INCLUDES
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(
  EXPORT argpppp-targets
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/argpppp
  CXX_MODULES_DIRECTORY .)

install(
  FILES ${PROJECT_SOURCE_DIR}/cmake/argpppp-config.cmake
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/argpppp)
