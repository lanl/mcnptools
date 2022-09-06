cmake_minimum_required(VERSION 3.12.1)
include_guard(DIRECTORY)

include("${CMAKE_CURRENT_LIST_DIR}/config.cmake")
if(shacl.cmake.installation)
  get_property(
    shacl.cmake.installed_modules GLOBAL PROPERTY shacl.cmake.installed_modules)

  if(NOT "DelegatingCacheVariable" IN_LIST shacl.cmake.installed_modules)
    set_property(GLOBAL APPEND PROPERTY
      shacl.cmake.installed_modules "DelegatingCacheVariable")

    install(
      FILES "${CMAKE_CURRENT_LIST_FILE}"
      DESTINATION share/cmake/shacl/.cmake)
  endif()

  unset(shacl.cmake.installed_modules)
endif()

include_guard(GLOBAL)
function(delegating_cache_variable variable)
  set(OPTIONS)
  set(UNARY_ARGUMENTS DEFAULT TYPE DOCSTRING)
  set(VARIADIC_ARGUMENTS)

  cmake_parse_arguments(dcv
    "${OPTIONS}"
    "${UNARY_ARGUMENTS}"
    "${VARIADIC_ARGUMENTS}" ${ARGN})

  if(NOT DEFINED dcv_DEFAULT)
    message(FATAL_ERROR
      "delegating_cache_variable invoked without 'DEFAULT' argument")
  endif()

  if(NOT DEFINED dcv_TYPE)
    message(FATAL_ERROR
      "delegating_cache_variable invoked without 'TYPE' argument")
  endif()

  set(valid_types FILEPATH PATH STRING BOOL INTERNAL)
  if(NOT dcv_TYPE IN_LIST valid_types)
    message("delegating_cache_variable error")
    message("TYPE argument value: ${dcv_TYPE}")
    message(FATAL_ERROR
      "TYPE argument must be one of FILEPATH, PATH, STRING, BOOL, or INTERNAL")
  endif()

  if(dcv_DOCSTRING)
    string(CONCAT docstring
      "${dcv_DOCSTRING}\n"
      "When set to 'default', ${variable} assumes the value of ${dcv_DEFAULT}")
  else()
    string(CONCAT docstring
      "When set to 'default', ${variable} assumes the value of ${dcv_DEFAULT}")
  endif()

  set(${variable} "default" CACHE ${dcv_TYPE} "${docstring}")

  if(${variable} STREQUAL "default")
    set(${variable} "${${dcv_DEFAULT}}" PARENT_SCOPE)
  endif()
endfunction()
