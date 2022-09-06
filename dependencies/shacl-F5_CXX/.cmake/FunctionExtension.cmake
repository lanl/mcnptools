cmake_minimum_required(VERSION 3.12.1)
include_guard(DIRECTORY)

include("${CMAKE_CURRENT_LIST_DIR}/config.cmake")
if(shacl.cmake.installation)
  get_property(
    shacl.cmake.installed_modules GLOBAL PROPERTY shacl.cmake.installed_modules)

  if(NOT "FunctionExtension" IN_LIST shacl.cmake.installed_modules)
    set_property(GLOBAL APPEND PROPERTY
      shacl.cmake.installed_modules "FunctionExtension")

    install(
      FILES "${CMAKE_CURRENT_LIST_FILE}"
      DESTINATION share/cmake/shacl/.cmake)

    install(
      DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/FunctionExtension"
      DESTINATION share/cmake/shacl/.cmake)
  endif()

  unset(shacl.cmake.installed_modules)
endif()

include_guard(GLOBAL)
include(FunctionExtension/push)
include(FunctionExtension/pop)
include(FunctionExtension/call)
include(FunctionExtension/backup)
include(FunctionExtension/add_library)
include(FunctionExtension/add_executable)
include(FunctionExtension/set_property)
include(FunctionExtension/set_target_properties)
include(FunctionExtension/target_sources)
include(FunctionExtension/find_package)
