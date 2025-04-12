# SPDX-FileCopyrightText: 2025 Thomas Mathys
# SPDX-License-Identifier: MIT

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# Install argpppp module and exports
# Based on https://crascit.com/2024/04/04/cxx-modules-cmake-shared-libraries/#h-installing-shared-libraries-with-c-20-modules
install(
  TARGETS argpppp
  EXPORT argppppTargets
  FILE_SET CXX_MODULES
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/argpppp/src
  FILE_SET HEADERS
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  INCLUDES
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(
  EXPORT argppppTargets
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/argpppp
  CXX_MODULES_DIRECTORY .)

# Generate and install argppppConfigVersion.cmake
write_basic_package_version_file(
  "${PROJECT_BINARY_DIR}/argppppConfigVersion.cmake"
  COMPATIBILITY SameMajorVersion)
install(
  FILES
  "${PROJECT_BINARY_DIR}/argppppConfigVersion.cmake"
  DESTINATION lib/cmake/argpppp)

# Generate and install argppppConfig.cmake
configure_package_config_file(
  "${PROJECT_SOURCE_DIR}/cmake/argppppConfig.cmake.in"
  "${PROJECT_BINARY_DIR}/argppppConfig.cmake"
  INSTALL_DESTINATION lib/cmake/argpppp)
install(
  FILES
  "${PROJECT_BINARY_DIR}/argppppConfig.cmake"
  DESTINATION lib/cmake/argpppp)
