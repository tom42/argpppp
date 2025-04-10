# SPDX-FileCopyrightText: 2025 Thomas Mathys
# SPDX-License-Identifier: MIT

# Install argpppp
# Based on https://crascit.com/2024/04/04/cxx-modules-cmake-shared-libraries/#h-installing-shared-libraries-with-c-20-modules
include(GNUInstallDirs)
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

install(
  FILES ${PROJECT_SOURCE_DIR}/cmake/argpppp-config.cmake
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/argpppp)
