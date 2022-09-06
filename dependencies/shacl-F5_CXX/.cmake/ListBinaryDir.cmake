cmake_minimum_required(VERSION 3.12.1)
include_guard(DIRECTORY)

include("${CMAKE_CURRENT_LIST_DIR}/config.cmake")
if(shacl.cmake.installation)
  get_property(
    shacl.cmake.installed_modules GLOBAL PROPERTY shacl.cmake.installed_modules)

  if(NOT "ListBinaryDir" IN_LIST shacl.cmake.installed_modules)
    set_property(GLOBAL APPEND PROPERTY
      shacl.cmake.installed_modules "ListBinaryDir")

    install(
      FILES "${CMAKE_CURRENT_LIST_FILE}"
      DESTINATION share/cmake/shacl/.cmake)
  endif()

  unset(shacl.cmake.installed_modules)
endif()

include_guard(GLOBAL)
function(LIST_BINARY_DIR_update variable access)
  if(${access} STREQUAL UNKNOWN_READ_ACCESS OR
     ${access} STREQUAL READ_ACCESS)

    file(RELATIVE_PATH
      relative_path
      "${CMAKE_SOURCE_DIR}"
      "${CMAKE_CURRENT_LIST_DIR}")

    if(relative_path)
      set(LIST_BINARY_DIR "${CMAKE_BINARY_DIR}/${relative_path}" PARENT_SCOPE)
    else()
      set(LIST_BINARY_DIR "${CMAKE_BINARY_DIR}" PARENT_SCOPE)
    endif()

  endif()
endfunction()

variable_watch(LIST_BINARY_DIR LIST_BINARY_DIR_update)
