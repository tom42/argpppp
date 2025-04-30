# SPDX-FileCopyrightText: 2025 Thomas Mathys
# SPDX-License-Identifier: MIT

# Test whether defining argp_program_version works.
# If it does, invoking the program with --version should succeed.
add_executable(argp_program_version_test argp_program_version_test.cpp)
vtg_target_enable_warnings(argp_program_version_test)
target_link_libraries(argp_program_version_test PRIVATE argpppp)
add_test(NAME argp_program_version_test COMMAND argp_program_version_test --version)
