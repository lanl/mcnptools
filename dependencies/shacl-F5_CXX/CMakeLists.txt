cmake_minimum_required(VERSION 3.13.0)

if(DEFINED PROJECT_NAME)
  set(subproject ON)
else()
  set(subproject OFF)
  set_property(GLOBAL PROPERTY USE_FOLDERS ON)
endif()

# C is required for find_package(HDF5) if HDF5 is not compiled via CMake
project(F5_CXX
  VERSION 1.6.2
  DESCRIPTION "A refreshing library for easy HDF5 usage"
  HOMEPAGE_URL "https://xcp-stash.lanl.gov/projects/SCL/repos/F5_CXX"
  LANGUAGES C CXX
)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/.cmake")

include(CMakeDependentOption)
include(CMakePackageConfigHelpers)
include(CTest)
include(DelegatingOption)
include(DependentDelegatingOption)
include(Git/Submodule/Packages)
include(Sanitizers)
include(Warnings)

if(EXISTS "${PROJECT_SOURCE_DIR}/cmake/specification.cmake")
  set(git.submodule.packages.specification
    "${PROJECT_SOURCE_DIR}/cmake/specification.cmake" CACHE FILEPATH
    "(READ ONLY) Path to consumed git submodule packages specification file" FORCE
  )
  include("${PROJECT_SOURCE_DIR}/cmake/specification.cmake")
endif()

dependent_delegating_option(shacl.F5_CXX.tests
  DEFAULT BUILD_TESTING
  DOCSTRING "Build the F5_CXX tests and integrate with ctest"
  CONDITION "NOT subproject"
  FALLBACK OFF
)

# Allow coarse-grain exclusion from INSTALLATION, with default of ON
CMAKE_DEPENDENT_OPTION(INSTALL_SUBPROJECTS
    "Perform installation of some subprojects"
    ON "subproject" ON)

dependent_delegating_option(shacl.F5_CXX.installation
  DEFAULT INSTALL_SUBPROJECTS
  DOCSTRING "Perform a full installation of F5_CXX"
  CONDITION "subproject"
  FALLBACK ON
)

add_library(F5_CXX INTERFACE)
add_library(shacl::F5_CXX ALIAS F5_CXX)

# Attempt to find CMake via the two approaches possible
# Via HDF5's recommended approach.
# see USING_HDF5_CMake.txt in HDF5 release_docs for more info.
message(STATUS "Attempting to find CMake-built HDF5")
find_package(HDF5 NAMES hdf5 COMPONENTS C)

if(HDF5_FOUND AND NOT DEFINED HDF5_IS_PARALLEL)
  set(HDF5_IS_PARALLEL ${HDF5_ENABLE_PARALLEL})

  # Older versions of HDF5 are missing the "hdf5::" prefix.  This attempts linkage to both.
  if(HDF5_USE_STATIC_LIBRARIES AND TARGET hdf5::hdf5-static)
    target_link_libraries(F5_CXX INTERFACE hdf5::hdf5-static)
  elseif(HDF5_USE_STATIC_LIBRARIES AND TARGET hdf5-static)
    target_link_libraries(F5_CXX INTERFACE hdf5-static)
  elseif(NOT HDF5_USE_STATIC_LIBRARIES AND TARGET hdf5::hdf5-shared)
    target_link_libraries(F5_CXX INTERFACE hdf5::hdf5-shared)
  elseif(NOT HDF5_USE_STATIC_LIBRARIES AND TARGET hdf5-shared)
    target_link_libraries(F5_CXX INTERFACE hdf5-shared)
  else()
    message(FATAL_ERROR "CMake found HDF5 but did not find a valid target.")
  endif()
else()
  # Via FindHDF5.cmake (HDF5_ROOT / h5cc checking)
  message(STATUS "Attempting to find makefile-built HDF5")
  find_package(HDF5 REQUIRED COMPONENTS C)

  target_link_libraries(F5_CXX INTERFACE ${HDF5_C_LIBRARIES})
  foreach(include_dir IN LISTS HDF5_INCLUDE_DIRS)
    # exclude "//include", which is erroneously included on some platforms for some spack-built HDF5 installations (Darwin P9).
    if (NOT ${include_dir} STREQUAL "//include")
      target_include_directories(F5_CXX INTERFACE $<BUILD_INTERFACE:${include_dir}>)
    endif()
  endforeach()
endif()

message(STATUS "Found HDF5. Version = ${HDF5_VERSION}, Is Parallel? = ${HDF5_IS_PARALLEL}")

if(HDF5_VERSION VERSION_LESS 1.10.2)
  message(FATAL_ERROR "HDF5 version too old. Found ${HDF5_VERSION}, need 1.10.2 or greater.")
endif()

CMAKE_DEPENDENT_OPTION(shacl.F5_CXX.MPI
  "Build the F5_CXX tests and library in parallel"
  ON "HDF5_IS_PARALLEL" OFF
)

mark_as_advanced(shacl.F5_CXX.MPI)

if(HDF5_IS_PARALLEL)
  find_package(MPI REQUIRED COMPONENTS C)
  target_link_libraries(F5_CXX INTERFACE MPI::MPI_C)
  if(shacl.F5_CXX.MPI)
    target_compile_definitions(F5_CXX INTERFACE PHDF5)
  else()
    message(STATUS "Serial F5_CXX selected, but linked against parallel HDF5")
  endif()
else()
  set(shacl.F5_CXX.MPI OFF)
  message(STATUS "No MPI parallel HDF5 found. Building serial library")
endif()

if(shacl.F5_CXX.tests)
  configure_file(
    "${PROJECT_SOURCE_DIR}/cmake/CTestCustom.cmake"
    "${PROJECT_BINARY_DIR}/CTestCustom.cmake"
    COPYONLY
  )

  find_package(Catch2 REQUIRED)

  set(CMAKE_CXX_EXTENSIONS OFF)

  add_library(shacl::F5_CXX::testing IMPORTED INTERFACE)

  set_target_properties(shacl::F5_CXX::testing PROPERTIES
    INTERFACE_WARN_ALL ON
    INTERFACE_WARN_ERROR ON
  )

  set_property(TARGET shacl::F5_CXX::testing APPEND PROPERTY
    COMPATIBLE_INTERFACE_BOOL
    WARN_ALL WARN_ERROR
  )

  target_link_libraries(shacl::F5_CXX::testing INTERFACE
    Catch2::Catch2
    shacl::cmake::Warnings_CXX
    shacl::cmake::Sanitizers_CXX
    shacl::F5_CXX
  )
endif()

target_compile_features(F5_CXX INTERFACE cxx_std_14)

include(GNUInstallDirs)
target_include_directories(F5_CXX INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/src>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

include(GNUInstallDirs)

string(CONCAT prefix
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/src>"
  "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

add_subdirectory(src/shacl)

if(shacl.F5_CXX.installation)
  install(DIRECTORY src/
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
    FILES_MATCHING PATTERN "*.hpp"
    PATTERN "*test*" EXCLUDE
  )

  install(FILES
    "${PROJECT_SOURCE_DIR}/.cmake/shacl-config.cmake"
    DESTINATION share/cmake/shacl
  )

  install(TARGETS F5_CXX EXPORT F5_CXXTargets)

  install(EXPORT F5_CXXTargets
    FILE shacl-f5_cxx-targets.cmake
    NAMESPACE shacl::
    DESTINATION share/cmake/shacl-F5_CXX
  )

  write_basic_package_version_file("shacl-f5_cxx-config-version.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
  )

  install(FILES
    "${PROJECT_BINARY_DIR}/shacl-f5_cxx-config-version.cmake"
    "${PROJECT_SOURCE_DIR}/cmake/shacl-f5_cxx-config.cmake"
    DESTINATION share/cmake/shacl-F5_CXX
  )

  if(NOT subproject)
    set(CPACK_PACKAGE_VENDOR "Los Alamos National Laboratory")
    set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
    set(CPACK_PACKAGE_CONTACT "cjosey@lanl.gov")
    include(CPack)
  endif()
endif()
