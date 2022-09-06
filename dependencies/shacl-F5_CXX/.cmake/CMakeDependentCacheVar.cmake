cmake_minimum_required(VERSION 3.12.1)
include_guard(DIRECTORY)

include("${CMAKE_CURRENT_LIST_DIR}/config.cmake")
if(shacl.cmake.installation)
  get_property(
    shacl.cmake.installed_modules GLOBAL PROPERTY shacl.cmake.installed_modules)

  if(NOT "CMakeDependentCacheVar" IN_LIST shacl.cmake.installed_modules)
    set_property(GLOBAL APPEND PROPERTY
      shacl.cmake.installed_modules "CMakeDependentCacheVar")

    install(
      FILES "${CMAKE_CURRENT_LIST_FILE}"
      DESTINATION share/cmake/shacl/.cmake)
  endif()

  unset(shacl.cmake.installed_modules)
endif()

include_guard(GLOBAL)
function(CMAKE_DEPENDENT_CACHE_VAR variable type docstring default conditions force)
  set(type_list FILEPATH PATH STRING BOOL INTERNAL)
  list(FIND type_list ${type} type_found)
  if( type_found LESS 0 )
    message("CMAKE_DEPENDENT_CACHE_VAR error: variable type")
    message(FATAL_ERROR
      "'${type}' must be one of FILEPATH, PATH, STRING, BOOL, or INTERNAL")
  endif()

  set(${variable}_AVAILABLE TRUE)
  foreach(condition ${conditions})
    string(REGEX REPLACE " +" ";"
      CMAKE_DEPENDENT_VARIABLE_DEP "${condition}")
    if(${CMAKE_DEPENDENT_VARIABLE_DEP})
    else()
      set(${variable}_AVAILABLE FALSE)
    endif()
  endforeach()

  if(${variable}_AVAILABLE)
    set(${variable} "${default}" CACHE "${type}" "${docstring}")
    set(${variable} "${${variable}}" CACHE "${type}" "${docstring}" FORCE)
  else()
    if(DEFINED ${variable})
      set(${variable} "${${variable}}" CACHE INTERNAL "${docstring}")
    endif()
    set(${variable} "${force}" PARENT_SCOPE)
  endif()
endfunction()
