# SPDX-FileCopyrightText: 2025 Thomas Mathys
# SPDX-License-Identifier: MIT

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# Install argpppp module and exports
# Based on https://crascit.com/2024/04/04/cxx-modules-cmake-shared-libraries/#h-installing-shared-libraries-with-c-20-modules
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

# Generate and install argpppp-config.cmake
configure_package_config_file(
  "${PROJECT_SOURCE_DIR}/cmake/argpppp-config.cmake.in"
  "${PROJECT_BINARY_DIR}/argpppp-config.cmake"
  INSTALL_DESTINATION lib/cmake/argpppp)
install(
  FILES "${PROJECT_BINARY_DIR}/argpppp-config.cmake"
  DESTINATION lib/cmake/argpppp)
