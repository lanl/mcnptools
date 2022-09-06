cmake_minimum_required(VERSION 3.12.1)
include_guard(DIRECTORY)

include("${CMAKE_CURRENT_LIST_DIR}/../config.cmake")
if(shacl.cmake.installation)
  get_property(
    shacl.cmake.installed_modules GLOBAL PROPERTY shacl.cmake.installed_modules)

  if(NOT "Fortran/ModuleDirectory" IN_LIST shacl.cmake.installed_modules)
    set_property(GLOBAL APPEND PROPERTY
      shacl.cmake.installed_modules "Fortran/ModuleDirectory")

    install(
      FILES "${CMAKE_CURRENT_LIST_FILE}"
      DESTINATION share/cmake/shacl/.cmake/Fortran)
  endif()

  unset(shacl.cmake.installed_modules)
endif()

include_guard(GLOBAL)
include(CMakeParseArguments)

function(target_Fortran_module_directory target linkage)
  set(OPTIONS)
  set(UNARY_ARGUMENTS BUILD_INTERFACE INSTALL_INTERFACE)
  set(VARIADIC_ARGUMENTS)
  cmake_parse_arguments(target_Fortran_module_directory
    "${OPTIONS}"
    "${UNARY_ARGUMENTS}"
    "${VARIADIC_ARGUMENTS}" ${ARGN})

  if(NOT target_Fortran_module_directory_BUILD_INTERFACE)
    message(FATAL_ERROR "target_Fortran_module_directory on target \"${target}\" missing BUILD_INTERFACE argument")
  endif()

  file(MAKE_DIRECTORY "${target_Fortran_module_directory_BUILD_INTERFACE}")

  set(multiconfig_suffix "")
  if(CMAKE_CONFIGURATION_TYPES)
    foreach(CONFIGURATION_TYPE IN LISTS CMAKE_CONFIGURATION_TYPES)
      file(MAKE_DIRECTORY "${target_Fortran_module_directory_BUILD_INTERFACE}/${CONFIGURATION_TYPE}")
    endforeach()
    set(multiconfig_suffix "/$<CONFIG>")
  endif()

  target_include_directories("${target}" "${linkage}"
    $<BUILD_INTERFACE:${target_Fortran_module_directory_BUILD_INTERFACE}${multiconfig_suffix}>)
  set_target_properties("${target}" PROPERTIES
    Fortran_MODULE_DIRECTORY ${target_Fortran_module_directory_BUILD_INTERFACE})

  if(target_Fortran_module_directory_INSTALL_INTERFACE)
    target_include_directories("${target}" "${linkage}"
      $<INSTALL_INTERFACE:${target_Fortran_module_directory_INSTALL_INTERFACE}>)

    install(
      DIRECTORY "${target_Fortran_module_directory_BUILD_INTERFACE}${multiconfig_suffix}/"
      DESTINATION "${target_Fortran_module_directory_INSTALL_INTERFACE}")
  endif()
endfunction()
