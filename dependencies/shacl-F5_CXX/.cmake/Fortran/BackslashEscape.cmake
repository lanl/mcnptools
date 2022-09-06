cmake_minimum_required(VERSION 3.12.1)
include_guard(DIRECTORY)

include("${CMAKE_CURRENT_LIST_DIR}/../config.cmake")

if(shacl.cmake.installation)
  get_property(
    shacl.cmake.installed_modules GLOBAL PROPERTY shacl.cmake.installed_modules)

  if(NOT "Fortran/BackslashEscape" IN_LIST shacl.cmake.installed_modules)
    set_property(GLOBAL APPEND PROPERTY
      shacl.cmake.installed_modules "Fortran/BackslashEscape")

    install(
      FILES "${CMAKE_CURRENT_LIST_FILE}"
      DESTINATION share/cmake/shacl/.cmake/Fortran)

    install(
      DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/BackslashEscape"
      DESTINATION share/cmake/shacl/.cmake/Fortran)
  endif()

  unset(shacl.cmake.installed_modules)
endif()

include_guard(GLOBAL)
enable_language(Fortran)

define_property(TARGET PROPERTY Fortran_BACKSLASH_ESCAPE
BRIEF_DOCS
"Allow for C-style backslash escape characters when reading and writing strings"
FULL_DOCS
"The behavior runs contrary to the Fortran Standard specification, but never the
less is a common vendor extension. This behavior defaults to OFF.")

add_library(shacl::cmake::Fortran::BackslashEscape INTERFACE IMPORTED GLOBAL)
set_property(TARGET shacl::cmake::Fortran::BackslashEscape
  APPEND PROPERTY COMPATIBLE_INTERFACE_STRING Fortran_BACKSLASH_ESCAPE)

include(Fortran/BackslashEscape/GNU)
include(Fortran/BackslashEscape/Intel)
include(Fortran/BackslashEscape/PGI)
