cmake_minimum_required(VERSION 3.12.1)
include_guard(DIRECTORY)

include("${CMAKE_CURRENT_LIST_DIR}/../config.cmake")
if(shacl.cmake.installation)
  get_property(
    shacl.cmake.installed_modules GLOBAL PROPERTY shacl.cmake.installed_modules)

  if(NOT "Fortran/Backtrace" IN_LIST shacl.cmake.installed_modules)
    set_property(GLOBAL APPEND PROPERTY
      shacl.cmake.installed_modules "Fortran/Backtrace")

    install(
      FILES "${CMAKE_CURRENT_LIST_FILE}"
      DESTINATION share/cmake/shacl/.cmake/Fortran)

    install(
      DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/Backtrace"
      DESTINATION share/cmake/shacl/.cmake/Fortran)
  endif()

  unset(shacl.cmake.installed_modules)
endif()

include_guard(GLOBAL)
enable_language(Fortran)

define_property(TARGET PROPERTY Fortran_BACKTRACE
BRIEF_DOCS
"Toggle backtrace information in Fortran programs"
FULL_DOCS
"This functionality is not universally supported amongst Fortran
compilers. This property provides an interface to that functionality
if available.")

add_library(shacl::cmake::Fortran::Backtrace INTERFACE IMPORTED GLOBAL)
set_property(TARGET shacl::cmake::Fortran::Backtrace
  APPEND PROPERTY COMPATIBLE_INTERFACE_BOOL Fortran_BACKTRACE)

include(Fortran/Backtrace/Intel)
include(Fortran/Backtrace/PGI)
include(Fortran/Backtrace/GNU)
