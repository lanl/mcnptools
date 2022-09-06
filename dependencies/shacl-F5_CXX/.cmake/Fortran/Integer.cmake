cmake_minimum_required(VERSION 3.12.1)

if(NOT TARGET shacl::cmake::Fortran::Integer::detail)
  add_library(shacl::cmake::Fortran::Integer::detail INTERFACE IMPORTED GLOBAL)
  target_compile_definitions(shacl::cmake::Fortran::Integer::detail INTERFACE
    $<$<EQUAL:$<TARGET_PROPERTY:Fortran_INTEGER_SIZE_BYTES>,4>:F90_INT_4BYTE>
    $<$<EQUAL:$<TARGET_PROPERTY:Fortran_INTEGER_SIZE_BYTES>,8>:F90_INT_8BYTE>)
  set_property(TARGET shacl::cmake::Fortran::Integer::detail
    APPEND PROPERTY COMPATIBLE_INTERFACE_STRING Fortran_INTEGER_SIZE_BYTES)
endif()

if(DEFINED CMAKE_C_COMPILER)
  if(NOT TARGET shacl::cmake::Fortran::Integer_C)
    add_library(shacl::cmake::Fortran::Integer_C INTERFACE IMPORTED GLOBAL)
    target_link_libraries(shacl::cmake::Fortran::Integer_C
      INTERFACE shacl::cmake::Fortran::Integer::detail)
  endif()
endif()

if(DEFINED CMAKE_CXX_COMPILER)
  if(NOT TARGET shacl::cmake::Fortran::Integer_CXX)
    add_library(shacl::cmake::Fortran::Integer_CXX INTERFACE IMPORTED GLOBAL)
    target_link_libraries(shacl::cmake::Fortran::Integer_CXX
      INTERFACE shacl::cmake::Fortran::Integer::detail)
  endif()
endif()

if(DEFINED CMAKE_Fortran_COMPILER)
  if(NOT TARGET shacl::cmake::Fortran::Integer_Fortran)
    add_library(shacl::cmake::Fortran::Integer_Fortran INTERFACE IMPORTED GLOBAL)
    target_link_libraries(shacl::cmake::Fortran::Integer_Fortran
      INTERFACE shacl::cmake::Fortran::Integer::detail)
  endif()
endif()

include(Fortran/Integer/GNU)
include(Fortran/Integer/Intel)
include(Fortran/Integer/PGI)

include_guard(DIRECTORY)
include("${CMAKE_CURRENT_LIST_DIR}/../config.cmake")
if(shacl.cmake.installation)
  get_property(
    shacl.cmake.installed_modules GLOBAL PROPERTY shacl.cmake.installed_modules)

  if(NOT "Fortran/Integer" IN_LIST shacl.cmake.installed_modules)
    set_property(GLOBAL APPEND PROPERTY
      shacl.cmake.installed_modules "Fortran/Integer")

    install(
      FILES "${CMAKE_CURRENT_LIST_FILE}"
      DESTINATION share/cmake/shacl/.cmake/Fortran)

    install(
      DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/Integer"
      DESTINATION share/cmake/shacl/.cmake/Fortran)
  endif()

  unset(shacl.cmake.installed_modules)
endif()

include_guard(GLOBAL)
define_property(TARGET PROPERTY Fortran_INTEGER_SIZE_BYTES
BRIEF_DOCS
"The size of the default Fortran integer in bytes"
FULL_DOCS
"This property specifies the size (in bytes) of the default Fortran integer,
e.g., in the expression 'integer :: i' where no kind is given")
