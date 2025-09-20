# SPDX-FileCopyrightText: 2025 Thomas Mathys
# SPDX-License-Identifier: MIT

# TODO: add example program
# TODO: do we want to have an option not to build the example? (And what's the default?)
add_executable(argpppp_example argpppp_example.cpp)
target_link_libraries(argpppp_example PRIVATE argpppp)
