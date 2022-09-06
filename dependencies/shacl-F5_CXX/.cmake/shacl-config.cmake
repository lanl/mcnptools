cmake_minimum_required(VERSION 3.12.1)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/.cmake")

if(shacl_FIND_QUIETLY)
  set(shacl_FIND_QUIETLY_ARG QUIET)
endif()

foreach(component IN LISTS shacl_FIND_COMPONENTS)
  if(${component} STREQUAL "cmake")
    set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" PARENT_SCOPE)
  else()
    set(shacl_FOUND FALSE) # (partial) work around for bug in
                           # find_dependency macro in cmake < 3.15.
    # For more information, see:
    # https://github.com/Kitware/CMake/commit/37da6af17d0d2cc8b499dc58f74866351e06c288#diff-a8dbf85f500c956187a26a08846639f0

    find_package(shacl-${component}
      ${shacl_FIND_QUIETLY_ARG}
      HINTS "${CMAKE_CURRENT_LIST_DIR}/../shacl-${component}")

    if(shacl_FIND_REQUIRED_${component} AND NOT shacl-${component}_FOUND)
      string(CONCAT
        shacl_NOT_FOUND_MESSAGE
        "shacl could not be found because dependency ${component}"
        " could not be found.")
      set(shacl_FOUND False)
      return()
    endif()
  endif()
endforeach()

set(shacl_FOUND TRUE)
