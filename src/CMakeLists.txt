# SPDX-FileCopyrightText: 2025 Thomas Mathys
# SPDX-License-Identifier: MIT

add_subdirectory(argpppp)

if(argpppp_BUILD_EXAMPLE)
  add_subdirectory(argpppp_example)
endif()
