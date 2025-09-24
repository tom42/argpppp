# SPDX-FileCopyrightText: 2025 Thomas Mathys
# SPDX-License-Identifier: MIT

# g++ bugs when <memory> and <filesystem> are used in the same translation unit.
# This seems to have gone away at some point, although I'm unsure with which version exactly.
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 15.0)
    add_compile_options(-Wno-attributes)
  endif()
endif()

add_subdirectory(argpppp_unit_test)
add_subdirectory(argpppp_test)
