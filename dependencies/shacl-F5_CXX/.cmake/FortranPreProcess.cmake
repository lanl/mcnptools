cmake_minimum_required(VERSION 3.12.1)
include_guard(DIRECTORY)

include("${CMAKE_CURRENT_LIST_DIR}/config.cmake")
if(shacl.cmake.installation)
  get_property(
    shacl.cmake.installed_modules GLOBAL PROPERTY shacl.cmake.installed_modules)

  if(NOT "FortranPreProcess" IN_LIST shacl.cmake.installed_modules)
    set_property(GLOBAL APPEND PROPERTY
      shacl.cmake.installed_modules "FortranPreProcess")

    install(
      FILES "${CMAKE_CURRENT_LIST_FILE}"
      DESTINATION share/cmake/shacl/.cmake)

    install(
      DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/FortranPreProcess"
      DESTINATION share/cmake/shacl/.cmake)
  endif()

  unset(shacl.cmake.installed_modules)
endif()

include(GeneratedSources)

include_guard(GLOBAL)
include(FortranPreProcess/FortranPreProcess)
include(FortranPreProcess/filename)
include(FortranPreProcess/target_sources)
