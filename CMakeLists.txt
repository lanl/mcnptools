cmake_minimum_required(VERSION 3.21)

cmake_policy(SET CMP0076 OLD)
project(mcnptools VERSION 5.3.1 LANGUAGES C CXX)

include(CTest)
include(CMakeDependentOption)
include(GNUInstallDirs)

option(mcnptools.cxx_extensions "allow use of C++ compiler extensions" OFF)
option(mcnptools.boost_regex "force use of boost_regex library" ON)
option(mcnptools.mpi "use mpi capabilities; requires system boost libraries" OFF)
option(mcnptools.python "build MCNPTools Python bindings" ON)
option(mcnptools.cpack "build cpack package" OFF)

get_directory_property(is_subproject PROPERTY PARENT_DIRECTORY)

# Because python links to a dynamic library, it is
# necessary to build static library dependencies w/ PIC.
CMAKE_DEPENDENT_OPTION(mcnptools.PIC
  "build mcnptools with position independent code bindings"
  "${CMAKE_POSITION_INDEPENDENT_CODE}"
  "NOT mcnptools.python" ON)

CMAKE_DEPENDENT_OPTION(mcnptools.testing
  "build MCNPTools tests"
  ON
  "BUILD_TESTING;NOT is_subproject" OFF)

set(
  mcnptools.python_install Global CACHE STRING
  "Select where to install python bindings. Global uses the global Python site-packages. User uses the user's site-packages. Prefix uses CMAKE_INSTALL_PREFIX and is recommended for deployment."
)
set_property(CACHE mcnptools.python_install PROPERTY STRINGS Global User Prefix)

################################################################################
# Add dependencies
################################################################################

set(Boost_NO_BOOST_CMAKE ON)
if(mcnptools.mpi)
  find_package(Boost 1.60 REQUIRED COMPONENTS serialization mpi)
  find_package(MPI REQUIRED)
endif()

find_package(Boost 1.60 QUIET COMPONENTS regex)

if(NOT Boost_REGEX_FOUND AND mcnptools.boost_regex)
  add_subdirectory(boost)
endif()

################################################################################
# Add components
################################################################################

add_subdirectory(libmcnptools)

# Setup RPaths correctly.
GET_TARGET_PROPERTY(INSTALL_RPATH ${PROJECT_NAME} INSTALL_RPATH)
SET_TARGET_PROPERTIES(
  ${PROJECT_NAME} PROPERTIES
  INSTALL_RPATH_USE_LINK_PATH OFF
  SKIP_BUILD_RPATH ON
  )

FILE(
  RELATIVE_PATH bin2lib
  "${CMAKE_INSTALL_FULL_BINDIR}"
  "${CMAKE_INSTALL_FULL_LIBDIR}"
  )

if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
  set(install_rpath "@executable_path;@executable_path/${bin2lib}")
else()
  set(install_rpath "$ORIGIN;$ORIGIN/${bin2lib}")
endif()

SET_PROPERTY(TARGET ${PROJECT_NAME} APPEND PROPERTY INSTALL_RPATH ${install_rpath})

add_subdirectory(dependencies)

add_subdirectory(utils)

if( mcnptools.python )
  add_subdirectory(python)
endif()

################################################################################
# CPACK
################################################################################

write_basic_package_version_file(
  "${PROJECT_NAME}-config-version.cmake"
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion
)

INSTALL(
  TARGETS ${PROJECT_NAME} EXPORT Config
  RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
  BUNDLE DESTINATION "."
)

INSTALL(
  EXPORT Config
  FILE "${PROJECT_NAME}-config.cmake"
  DESTINATION "share/cmake/${PROJECT_NAME}"
)

INSTALL(
  FILES "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake"
  DESTINATION "share/cmake/${PROJECT_NAME}"
)


if((NOT subproject) AND mcnptools.cpack)
  ADD_SUBDIRECTORY(install)
  SET(CPACK_PACKAGE_VENDOR "Los Alamos National Laboratory")
  SET(CPACK_STRIP_FILES "ON")
  SET(CPACK_PACKAGE_INSTALL_DIRECTORY "${PROJECT_NAME}-${PROJECT_VERSION}")
  SET(CPACK_PACKAGE_INSTALL_REGISTRY_KEY "${PROJECT_NAME}-${PROJECT_VERSION}")

  SET(CPACK_NSIS_MODIFY_PATH "ON")
  SET(CPACK_NSIS_DISPLAY_NAME "${PROJECT_NAME}-${PROJECT_VERSION}")
  SET(CPACK_NSIS_DEFINES "RequestExecutionLevel user")

  INCLUDE(CPack)
endif()
