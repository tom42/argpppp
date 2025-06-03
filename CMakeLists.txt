# SPDX-FileCopyrightText: 2025 Thomas Mathys
# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.31.5)
project(argpppp VERSION 0.0.7 LANGUAGES CXX)
list(INSERT CMAKE_MODULE_PATH 0 "${CMAKE_CURRENT_LIST_DIR}/cmake")


################################################################################
# Options
################################################################################

option(argpppp_BUILD_TESTING "Build argpppp tests" ${PROJECT_IS_TOP_LEVEL})
option(argpppp_ENABLE_WARNINGS "Build argpppp with warnings enabled" OFF)
option(argpppp_INSTALL_STATIC_LIBRARY "Install the argpppp static library" ${PROJECT_IS_TOP_LEVEL})


################################################################################
# Set up testing
################################################################################

if(argpppp_BUILD_TESTING)
  enable_testing()
  include(VtgTesting)
  vtg_testing_setup_catch2(3.7.1)
endif()


################################################################################
# Build our own code
################################################################################

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# argpppp_buildflags' sole purpose is to carry build flags.
# This gives us a single place where we can define build flags.
# Other projects can use the build flags by privately linking to this library.
# Based on https://discourse.cmake.org/t/setting-cmake-cxx-flags-within-a-function/4163/7
#
# If a target that uses argpppp_buildflags is installed, this requires
# argpppp_buildflags to be installed too. This is not exactly pretty.
# No file is actually installed for an empty interface library,
# but argpppp_buildflags does show up in the installed argppppTargets.cmake.
add_library(argpppp_buildflags INTERFACE)
if(argpppp_ENABLE_WARNINGS)
  include(VtgEnableWarnings)
  vtg_target_enable_warnings(argpppp_buildflags)
endif()

add_subdirectory(src)
if(argpppp_BUILD_TESTING)
  add_subdirectory(test)
endif()


################################################################################
# Add custom targets (reuse lint etc.)
################################################################################

if(PROJECT_IS_TOP_LEVEL)
  add_custom_target(
    reuse-lint
    COMMAND reuse lint
    WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}")
endif()
