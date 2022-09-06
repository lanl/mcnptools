cmake_minimum_required(VERSION 3.12.1)
include_guard(DIRECTORY)

include("${CMAKE_CURRENT_LIST_DIR}/config.cmake")
if(shacl.cmake.installation)
  get_property(
    shacl.cmake.installed_modules GLOBAL PROPERTY shacl.cmake.installed_modules)

  if(NOT "DependentDelegatingCacheVariable" IN_LIST shacl.cmake.installed_modules)
    set_property(GLOBAL APPEND PROPERTY
      shacl.cmake.installed_modules "DependentDelegatingCacheVariable")

    install(
      FILES "${CMAKE_CURRENT_LIST_FILE}"
      DESTINATION share/cmake/shacl/.cmake)
  endif()

  unset(shacl.cmake.installed_modules)
endif()

include_guard(GLOBAL)
function(dependent_delegating_cache_variable variable)
  set(OPTIONS)
  set(UNARY_ARGUMENTS DEFAULT TYPE DOCSTRING CONDITION FALLBACK)
  set(VARIADIC_ARGUMENTS)

  cmake_parse_arguments(ddcv
    "${OPTIONS}"
    "${UNARY_ARGUMENTS}"
    "${VARIADIC_ARGUMENTS}" ${ARGN})

  if(NOT DEFINED ddcv_DEFAULT)
    message(FATAL_ERROR
      "dependent_delegating_cache_variable invoked without 'DEFAULT' argument")
  endif()

  if(NOT DEFINED ddcv_TYPE)
    message(FATAL_ERROR
      "dependent_delegating_cache_variable invoked without 'TYPE' argument")
  endif()

  set(valid_types FILEPATH PATH STRING BOOL INTERNAL)
  if(NOT ddcv_TYPE IN_LIST valid_types)
    message("dependent_delegating_cache_variable error")
    message("TYPE argument value: ${ddcv_TYPE}")
    message(FATAL_ERROR
      "TYPE argument must be one of FILEPATH, PATH, STRING, BOOL, or INTERNAL")
  endif()

  if(ddcv_DOCSTRING)
    string(CONCAT docstring
      "${ddcv_DOCSTRING}\n"
      "When set to 'default', ${variable} assumes the value of ${ddcv_DEFAULT}")
  else()
    string(CONCAT docstring
      "When set to 'default', ${variable} assumes the value of ${ddcv_DEFAULT}")
  endif()

  if(NOT DEFINED ddcv_CONDITION)
    message(FATAL_ERROR
      "dependent_delegating_cache_variable invoked without 'CONDITION' argument")
  endif()

  if(NOT DEFINED ddcv_FALLBACK)
    message(FATAL_ERROR
      "dependent_delegating_cache_variable invoked without 'FALLBACK' argument")
  endif()

  set(available TRUE)
  foreach(condition IN LISTS ddcv_CONDITION)
    string(REGEX REPLACE " +" ";" condition "${condition}")
    if(${condition})
    else()
      set(available FALSE)
      break()
    endif()
  endforeach()

  if(${available})
    set(${variable} "default" CACHE "${ddcv_TYPE}" "${docstring}")
    set(${variable} "${${variable}}" CACHE "${ddcv_TYPE}" "${docstring}" FORCE)
    if(${variable} STREQUAL "default")
      set(${variable} "${${ddcv_DEFAULT}}" PARENT_SCOPE)
    endif()
  else()
    if(DEFINED ${variable})
      set(${variable} "${${variable}}" CACHE INTERNAL "${docstring}")
    endif()
    set(${variable} "${ddcv_FALLBACK}" PARENT_SCOPE)
  endif()
endfunction()
