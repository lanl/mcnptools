cmake_minimum_required(VERSION 3.12.1)
include_guard(DIRECTORY)

include("${CMAKE_CURRENT_LIST_DIR}/../config.cmake")
if(shacl.cmake.installation)
  get_property(
    shacl.cmake.installed_modules GLOBAL PROPERTY shacl.cmake.installed_modules)

  if(NOT "Fortran/Standard" IN_LIST shacl.cmake.installed_modules)
    set_property(GLOBAL APPEND PROPERTY
      shacl.cmake.installed_modules "Fortran/Standard")

    install(
      FILES "${CMAKE_CURRENT_LIST_FILE}"
      DESTINATION share/cmake/shacl/.cmake/Fortran)

    install(
      DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/Standard"
      DESTINATION share/cmake/shacl/.cmake/Fortran)
  endif()

  unset(shacl.cmake.installed_modules)
endif()

if(NOT CMAKE_Fortran_COMPILER)
  return()
endif()

include_guard(GLOBAL)
define_property(TARGET PROPERTY Fortran_STANDARD
BRIEF_DOCS
"The Fortran standard whose features are requested to build this target."
FULL_DOCS
"This property specifies the Fortran standard whose features are requested to
 build this target. For some compilers, this results in adding a flag such as
 -std=f2003 to the compile line. For compilers that have no notion of a
 standard level, such as PGI, this has no effect.

 Supported values are 95, 2003, 2008, and 2018")

add_library(shacl::cmake::Fortran::Standard INTERFACE IMPORTED GLOBAL)
set_property(TARGET shacl::cmake::Fortran::Standard
  APPEND PROPERTY COMPATIBLE_INTERFACE_STRING Fortran_STANDARD)

include(Fortran/Standard/GNU)
include(Fortran/Standard/Intel)
