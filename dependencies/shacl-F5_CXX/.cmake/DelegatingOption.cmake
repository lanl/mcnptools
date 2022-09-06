cmake_minimum_required(VERSION 3.12.1)
include_guard(DIRECTORY)

include("${CMAKE_CURRENT_LIST_DIR}/config.cmake")

if(shacl.cmake.installation)
  get_property(
    shacl.cmake.installed_modules GLOBAL PROPERTY shacl.cmake.installed_modules)

  if(NOT "DelegatingOption" IN_LIST shacl.cmake.installed_modules)
    set_property(GLOBAL APPEND PROPERTY
      shacl.cmake.installed_modules "DelegatingOption")

    install(
      FILES "${CMAKE_CURRENT_LIST_FILE}"
      DESTINATION share/cmake/shacl/.cmake)
  endif()

  unset(shacl.cmake.installed_modules)
endif()

include_guard(GLOBAL)
function(delegating_option variable)
  set(OPTIONS)
  set(UNARY_ARGUMENTS DEFAULT DOCSTRING)
  set(VARIADIC_ARGUMENTS)

  cmake_parse_arguments(do
    "${OPTIONS}"
    "${UNARY_ARGUMENTS}"
    "${VARIADIC_ARGUMENTS}" ${ARGN})

  if(NOT DEFINED do_DEFAULT)
    message(FATAL_ERROR
      "delegating_option invoked without 'DEFAULT' argument")
  endif()

  if(do_DOCSTRING)
    string(CONCAT docstring
      "${do_DOCSTRING}\n"
      "When set to 'default', ${variable} assumes the value of ${do_DEFAULT}")
  else()
    string(CONCAT docstring
      "When set to 'default', ${variable} assumes the value of ${do_DEFAULT}")
  endif()

  set(${variable} "default" CACHE STRING "${docstring}")
  set_property(CACHE ${variable} PROPERTY STRINGS default ON OFF)
  if(${variable} STREQUAL "default")
    if(${do_DEFAULT})
      set(${variable} ON PARENT_SCOPE)
    else()
      set(${variable} OFF PARENT_SCOPE)
    endif()
  endif()
endfunction()
