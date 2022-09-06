cmake_minimum_required(VERSION 3.12.1)
include_guard(DIRECTORY)

include("${CMAKE_CURRENT_LIST_DIR}/../config.cmake")
if(shacl.cmake.installation)
  get_property(
    shacl.cmake.installed_modules GLOBAL PROPERTY shacl.cmake.installed_modules)

  if(NOT "Fortran/Exceptions" IN_LIST shacl.cmake.installed_modules)
    set_property(GLOBAL APPEND PROPERTY
      shacl.cmake.installed_modules "Fortran/Exceptions")

    install(
      FILES "${CMAKE_CURRENT_LIST_FILE}"
      DESTINATION share/cmake/shacl/.cmake/Fortran)

    install(
      DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/Exceptions"
      DESTINATION share/cmake/shacl/.cmake/Fortran)
  endif()

  unset(shacl.cmake.installed_modules)
endif()

include_guard(GLOBAL)
enable_language(Fortran)

add_library(shacl::cmake::Fortran::Exceptions INTERFACE IMPORTED GLOBAL)

include(Fortran/Exceptions/GNU)
include(Fortran/Exceptions/Intel)
