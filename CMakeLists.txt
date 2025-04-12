# SPDX-FileCopyrightText: 2025 Thomas Mathys
# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.31.5)
project(argpppp VERSION 0.0.1 LANGUAGES CXX)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

include(CTest)

# Build all 3rd party code before globally raising warning levels.
if(BUILD_TESTING)
  include(VtgTesting)
  vtg_testing_setup_catch2(3.7.1)
endif()

# Raise warning levels. From now on we should only build our own code.
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
include(VtgEnableWarnings)

add_subdirectory(src)
if(BUILD_TESTING)
  add_subdirectory(test)
endif()

add_custom_target(
  reuse-lint
  COMMAND reuse lint
  WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}")
