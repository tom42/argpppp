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

# Unit testing version of argpppp
if(BUILD_TESTING)
  add_library(argpppp_unit_testing)
  target_sources(argpppp_unit_testing ${argpppp_sources})
  target_compile_definitions(argpppp_unit_testing PRIVATE ARGPPPP_EXPORT_FOR_UNIT_TESTING=export)
endif()
