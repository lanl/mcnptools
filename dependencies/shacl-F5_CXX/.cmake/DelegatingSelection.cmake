cmake_minimum_required(VERSION 3.12.1)
include_guard(DIRECTORY)

include("${CMAKE_CURRENT_LIST_DIR}/config.cmake")
if(shacl.cmake.installation)
  get_property(
    shacl.cmake.installed_modules GLOBAL PROPERTY shacl.cmake.installed_modules)

  if(NOT "DelegatingSelection" IN_LIST shacl.cmake.installed_modules)
    set_property(GLOBAL APPEND PROPERTY
      shacl.cmake.installed_modules "DelegatingSelection")

    install(
      FILES "${CMAKE_CURRENT_LIST_FILE}"
      DESTINATION share/cmake/shacl/.cmake)
  endif()

  unset(shacl.cmake.installed_modules)
endif()

include_guard(GLOBAL)
function(delegating_selection variable)
  set(OPTIONS)
  set(UNARY_ARGUMENTS DEFAULT DOCSTRING)
  set(VARIADIC_ARGUMENTS OPTIONS)

  cmake_parse_arguments(ds
    "${OPTIONS}"
    "${UNARY_ARGUMENTS}"
    "${VARIADIC_ARGUMENTS}" ${ARGN})

  if(NOT DEFINED ds_DEFAULT)
    message(FATAL_ERROR
      "delegating_selection invoked without 'DEFAULT' argument")
  endif()

  if(ds_DOCSTRING)
    string(CONCAT docstring
      "${ds_DOCSTRING}\n"
      "When set to 'default', ${variable} assumes the value of ${ds_DEFAULT}")
  else()
    string(CONCAT docstring
      "When set to 'default', ${variable} assumes the value of ${ds_DEFAULT}")
  endif()

  if(NOT DEFINED ds_OPTIONS)
    message(FATAL_ERROR "selection invoked without 'OPTIONS' keyword")
  endif()

  set(${variable} "default" CACHE STRING "${docstring}")
  set(options default;${ds_OPTIONS})
  set_property(CACHE ${variable} PROPERTY STRINGS ${options})

  if(${variable} STREQUAL "default")
    set(${variable} "${${ds_DEFAULT}}")
  endif()

  if(NOT ${variable} IN_LIST options)
    message("${variable} set to ${${variable}}")
    set(error_message "${variable} must be one of:")
    foreach(option_string IN LISTS options)
      string(CONCAT error_message "${error_message}\n" "  ${option_string}")
    endforeach()
    message(FATAL_ERROR "${error_message}")
  endif()

  set(${variable} "${${variable}}" PARENT_SCOPE)
endfunction()
