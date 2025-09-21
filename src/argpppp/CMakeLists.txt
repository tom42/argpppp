# SPDX-FileCopyrightText: 2025 Thomas Mathys
# SPDX-License-Identifier: MIT

set(
  argpppp_sources
  PUBLIC FILE_SET CXX_MODULES FILES
  argpppp.cppm
  command_line_parser.cppm
  of.cppm
  option.cppm
  option_error.cppm
  option_handlers.cppm
  optional_string.cppm
  options.cppm
  pf.cppm
  string_to_floating_point_converter.cppm
  string_to_integral_converter.cppm
  PRIVATE
  command_line_parser.cpp
  option.cpp
  options.cpp)

# Production version of argpppp
add_library(argpppp)
target_sources(argpppp ${argpppp_sources})
target_compile_definitions(argpppp PRIVATE ARGPPPP_EXPORT_FOR_UNIT_TESTING=)
target_compile_features(argpppp PUBLIC cxx_std_23)
vtg_target_enable_warnings(argpppp)

# Unit testing version of argpppp
if(argpppp_BUILD_TESTING)
  add_library(argpppp_unit_testing)
  target_sources(argpppp_unit_testing ${argpppp_sources})
  target_compile_definitions(argpppp_unit_testing PRIVATE ARGPPPP_EXPORT_FOR_UNIT_TESTING=export)
  target_compile_features(argpppp_unit_testing PUBLIC cxx_std_23)
  vtg_target_enable_warnings(argpppp_unit_testing)
endif()

# Check for argp_parse
include(CheckSymbolExists)
check_symbol_exists(argp_parse "argp.h" argpppp_HAVE_ARGP)

# Use argp-standalone if argp_parse is not around
if(NOT argpppp_HAVE_ARGP)
  include(FetchContent)
  FetchContent_Declare(
    argp-standalone
    GIT_REPOSITORY https://github.com/tom42/argp-standalone
    GIT_TAG        v1.0.3)
  FetchContent_MakeAvailable(argp-standalone)
  target_link_libraries(argpppp PRIVATE argp-standalone)
  if(argpppp_BUILD_TESTING)
    target_link_libraries(argpppp_unit_testing PRIVATE argp-standalone)
  endif()
endif()

# Installation does not yet work for environments that need argp-standalone
if(argpppp_INSTALL_STATIC_LIBRARY AND argpppp_HAVE_ARGP)
  include(InstallArgpppp)
endif()
