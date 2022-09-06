cmake_minimum_required(VERSION 3.12.1)

if(NOT TARGET shacl::cmake::Fortran::Real::detail)
  add_library(shacl::cmake::Fortran::Real::detail INTERFACE IMPORTED GLOBAL)
  target_compile_definitions(shacl::cmake::Fortran::Real::detail INTERFACE
    $<$<EQUAL:$<TARGET_PROPERTY:Fortran_REAL_SIZE_BYTES>,4>:F90_REAL_4BYTE>
    $<$<EQUAL:$<TARGET_PROPERTY:Fortran_REAL_SIZE_BYTES>,8>:F90_REAL_8BYTE>)
  set_property(TARGET shacl::cmake::Fortran::Real::detail
    APPEND PROPERTY COMPATIBLE_INTERFACE_STRING Fortran_REAL_SIZE_BYTES)
endif()

if(DEFINED CMAKE_C_COMPILER)
  if(NOT TARGET shacl::cmake::Fortran::Real_C)
    add_library(shacl::cmake::Fortran::Real_C INTERFACE IMPORTED GLOBAL)
    target_link_libraries(shacl::cmake::Fortran::Real_C
      INTERFACE shacl::cmake::Fortran::Real::detail)
  endif()
endif()

if(DEFINED CMAKE_CXX_COMPILER)
  if(NOT TARGET shacl::cmake::Fortran::Real_CXX)
    add_library(shacl::cmake::Fortran::Real_CXX INTERFACE IMPORTED GLOBAL)
    target_link_libraries(shacl::cmake::Fortran::Real_CXX
      INTERFACE shacl::cmake::Fortran::Real::detail)
  endif()
endif()

if(DEFINED CMAKE_Fortran_COMPILER)
  if(NOT TARGET shacl::cmake::Fortran::Real_Fortran)
    add_library(shacl::cmake::Fortran::Real_Fortran INTERFACE IMPORTED GLOBAL)
    target_link_libraries(shacl::cmake::Fortran::Real_Fortran
      INTERFACE shacl::cmake::Fortran::Real::detail)
  endif()
endif()

include(Fortran/Real/GNU)
include(Fortran/Real/Intel)
include(Fortran/Real/PGI)

include_guard(DIRECTORY)
include("${CMAKE_CURRENT_LIST_DIR}/../config.cmake")
if(shacl.cmake.installation)
  get_property(
    shacl.cmake.installed_modules GLOBAL PROPERTY shacl.cmake.installed_modules)

  if(NOT "Fortran/Real" IN_LIST shacl.cmake.installed_modules)
    set_property(GLOBAL APPEND PROPERTY
      shacl.cmake.installed_modules "Fortran/Real")

    install(
      FILES "${CMAKE_CURRENT_LIST_FILE}"
      DESTINATION share/cmake/shacl/.cmake/Fortran)

    install(
      DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/Real"
      DESTINATION share/cmake/shacl/.cmake/Fortran)
  endif()

  unset(shacl.cmake.installed_modules)
endif()

include_guard(GLOBAL)
define_property(TARGET PROPERTY Fortran_REAL_SIZE_BYTES
BRIEF_DOCS
"The size of the default Fortran real in bytes"
FULL_DOCS
"This property specifies the size (in bytes) of the default Fortran real, e.g.,
in the expression 'real :: r' where no kind is given")
