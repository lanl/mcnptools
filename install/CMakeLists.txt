SET(CMAKE_INSTALL_SYSTEM_RUNTIME_DESTINATION "${CMAKE_INSTALL_LIBDIR}")

# InstallRequiredSystemLibraries determines the paths to libraries
# by searching a set of paths relative to the compiler path. However
# it does not resolve symlinks to real paths as part of the process.
# As such, these sibling directories are unlikely to be found when
# symlinks are used. Here was set several directory variables (which
# hide the cache variables) such that the sibling directories will be
# found.

GET_FILENAME_COMPONENT(CMAKE_C_COMPILER "${CMAKE_C_COMPILER}" REALPATH)
GET_FILENAME_COMPONENT(CMAKE_CXX_COMPILER "${CMAKE_CXX_COMPILER}" REALPATH)
GET_FILENAME_COMPONENT(CMAKE_Fortran_COMPILER "${CMAKE_Fortran_COMPILER}" REALPATH)

INCLUDE(InstallRequiredSystemLibraries)

CONFIGURE_FILE(
 "${CMAKE_CURRENT_SOURCE_DIR}/bundle_dependencies.cmake.in"
 "${CMAKE_CURRENT_BINARY_DIR}/bundle_dependencies.cmake.in"
 @ONLY
)

FILE(
  GENERATE
  OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/bundle_dependencies.cmake"
  INPUT  "${CMAKE_CURRENT_BINARY_DIR}/bundle_dependencies.cmake.in"
)

INSTALL(SCRIPT "${CMAKE_CURRENT_BINARY_DIR}/bundle_dependencies.cmake")
