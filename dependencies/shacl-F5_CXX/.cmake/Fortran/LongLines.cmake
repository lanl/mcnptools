cmake_minimum_required(VERSION 3.10.1)
include_guard(DIRECTORY)

include("${CMAKE_CURRENT_LIST_DIR}/../config.cmake")
if(shacl.cmake.installation)
  get_property(
    shacl.cmake.installed_modules GLOBAL PROPERTY shacl.cmake.installed_modules)

  if(NOT "Fortran/LongLines" IN_LIST shacl.cmake.installed_modules)
    set_property(GLOBAL APPEND PROPERTY
      shacl.cmake.installed_modules "Fortran/LongLines")

    install(
      FILES "${CMAKE_CURRENT_LIST_FILE}"
      DESTINATION share/cmake/shacl/.cmake/Fortran)

    install(
      DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/LongLines"
      DESTINATION share/cmake/shacl/.cmake/Fortran)
  endif()

  unset(shacl.cmake.installed_modules)
endif()

include_guard(GLOBAL)
enable_language(Fortran)
add_library(shacl::cmake::Fortran::LongLines INTERFACE IMPORTED GLOBAL)

include(Fortran/LongLines/GNU)
