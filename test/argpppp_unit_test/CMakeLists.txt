# SPDX-FileCopyrightText: 2025 Thomas Mathys
# SPDX-License-Identifier: MIT

add_executable(
  argpppp_unit_test
  command_line_parser_test.cpp
  of_test.cpp
  option_test.cpp
  optional_string_test.cpp
  options_test.cpp
  parse_number_test.cpp
  parse_result_test.cpp
  pf_test.cpp
  string_to_floating_point_converter_test.cpp)
vtg_target_enable_warnings(argpppp_unit_test)
target_link_libraries(argpppp_unit_test PRIVATE argpppp_unit_testing Catch2::Catch2WithMain)
catch_discover_tests(argpppp_unit_test)
