cmake_minimum_required(VERSION 3.12.1)
include_guard(DIRECTORY)

include("${CMAKE_CURRENT_LIST_DIR}/../config.cmake")
if(shacl.cmake.installation)
  get_property(
    shacl.cmake.installed_modules GLOBAL PROPERTY shacl.cmake.installed_modules)

  if(NOT "LinkOptions/StackSize" IN_LIST shacl.cmake.installed_modules)
    set_property(GLOBAL APPEND PROPERTY
      shacl.cmake.installed_modules "LinkOptions/StackSize")

    install(
      FILES "${CMAKE_CURRENT_LIST_FILE}"
      DESTINATION share/cmake/shacl/.cmake/LinkOptions)

    install(
      DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/StackSize"
      DESTINATION share/cmake/shacl/.cmake/LinkOptions)
  endif()

  unset(shacl.cmake.installed_modules)
endif()

include_guard(GLOBAL)
define_property(TARGET PROPERTY LINK_STACK_SIZE
  BRIEF_DOCS "minimum link-time stack size in bytes to be set by the linker"
  FULL_DOCS "minimum link-time stack size in bytes to be set by the linker")

add_library(shacl::cmake::LinkOptions::StackSize INTERFACE IMPORTED GLOBAL)

include(CheckLanguage)
check_language(Fortran)

include(LinkOptions/StackSize/GNU)
include(LinkOptions/StackSize/Intel)
include(LinkOptions/StackSize/PGI)
include(LinkOptions/StackSize/LLVM)
include(LinkOptions/StackSize/MSVC)
