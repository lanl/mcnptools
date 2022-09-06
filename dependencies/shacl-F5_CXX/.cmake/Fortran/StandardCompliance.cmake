cmake_minimum_required(VERSION 3.12.1)
include_guard(DIRECTORY)

include("${CMAKE_CURRENT_LIST_DIR}/../config.cmake")
if(shacl.cmake.installation)
  get_property(
    shacl.cmake.installed_modules GLOBAL PROPERTY shacl.cmake.installed_modules)

  if(NOT "Fortran/StandardCompliance" IN_LIST shacl.cmake.installed_modules)
    set_property(GLOBAL APPEND PROPERTY
      shacl.cmake.installed_modules "Fortran/StandardCompliance")

    install(
      FILES "${CMAKE_CURRENT_LIST_FILE}"
      DESTINATION share/cmake/shacl/.cmake/Fortran)

    install(
      DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/StandardCompliance"
      DESTINATION share/cmake/shacl/.cmake/Fortran)
  endif()

  unset(shacl.cmake.installed_modules)
endif()

include(Fortran/BackslashEscape)

include_guard(GLOBAL)
enable_language(Fortran)

add_library(shacl::cmake::Fortran::StandardCompliance INTERFACE IMPORTED GLOBAL)

target_link_libraries(shacl::cmake::Fortran::StandardCompliance INTERFACE
  shacl::cmake::Fortran::BackslashEscape)

set_target_properties(shacl::cmake::Fortran::StandardCompliance PROPERTIES
  INTERFACE_Fortran_BACKSLASH_ESCAPE OFF)

include(Fortran/StandardCompliance/GNU)
include(Fortran/StandardCompliance/Intel)
include(Fortran/StandardCompliance/PGI)
