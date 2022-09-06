cmake_minimum_required(VERSION 3.12.1)
include_guard(DIRECTORY)

include("${CMAKE_CURRENT_LIST_DIR}/config.cmake")
if(shacl.cmake.installation)
  get_property(
    shacl.cmake.installed_modules GLOBAL PROPERTY shacl.cmake.installed_modules)

  if(NOT "Fortran" IN_LIST shacl.cmake.installed_modules)
    set_property(GLOBAL APPEND PROPERTY shacl.cmake.installed_modules "Fortran")

    install(
      FILES "${CMAKE_CURRENT_LIST_FILE}"
      DESTINATION share/cmake/shacl/.cmake)

    install(
      FILES "${CMAKE_CURRENT_LIST_DIR}/README.md"
      DESTINATION share/cmake/shacl/.cmake)
  endif()

  unset(shacl.cmake.installed_modules)
endif()

include(Fortran/BackslashEscape)
include(Fortran/Backtrace)
include(Fortran/Exceptions)
include(Fortran/Integer)
include(Fortran/ModuleDirectory)
include(Fortran/Real)
include(Fortran/StackArraySizeLimit)
include(Fortran/Standard)
include(Fortran/StandardCompliance)
