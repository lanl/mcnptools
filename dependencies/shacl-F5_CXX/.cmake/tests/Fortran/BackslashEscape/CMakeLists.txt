include(Fortran)

file(GENERATE OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/use_backslash_escape.F90"
              INPUT "${CMAKE_CURRENT_LIST_DIR}/use_backslash_escape.F90.in")

add_executable(use_backslash_escape ${CMAKE_CURRENT_BINARY_DIR}/use_backslash_escape.F90)
set_target_properties(use_backslash_escape PROPERTIES Fortran_BACKSLASH_ESCAPE ON)
target_link_libraries(use_backslash_escape PRIVATE shacl::cmake::Fortran::BackslashEscape)

set(expected "")

if("${CMAKE_Fortran_COMPILER_ID}" STREQUAL "PGI")
  set(expected "-Mbackslash")
elseif("${CMAKE_Fortran_COMPILER_ID}" STREQUAL "GNU")
  set(expected "-fbackslash")
elseif("${CMAKE_Fortran_COMPILER_ID}" STREQUAL "Intel")
  if(WIN32)
    set(expected "/assume:bscc")
  else()
    set(expected "SHELL:-assume bscc")
  endif()
endif()

add_test(
  NAME shacl.cmake.backslash_escape
  COMMAND use_backslash_escape)

set_tests_properties(shacl.cmake.backslash_escape PROPERTIES PASS_REGULAR_EXPRESSION "^${expected}\n$")

file(GENERATE OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/use_no_backslash_escape.F90"
              INPUT "${CMAKE_CURRENT_LIST_DIR}/use_no_backslash_escape.F90.in")

add_executable(use_no_backslash_escape ${CMAKE_CURRENT_BINARY_DIR}/use_no_backslash_escape.F90)
set_target_properties(use_no_backslash_escape PROPERTIES Fortran_BACKSLASH_ESCAPE OFF)
target_link_libraries(use_no_backslash_escape PRIVATE shacl::cmake::Fortran::BackslashEscape)

set(expected "")

if("${CMAKE_Fortran_COMPILER_ID}" STREQUAL "PGI")
  set(expected "-Mnobackslash\n")
elseif("${CMAKE_Fortran_COMPILER_ID}" STREQUAL "GNU")
  set(expected "")
elseif("${CMAKE_Fortran_COMPILER_ID}" STREQUAL "Intel")
  if(WIN32)
    set(expected "/assume:nobscc\n")
  else()
    set(expected "SHELL:-assume nobscc\n")
  endif()
endif()

add_test(
  NAME shacl.cmake.no_backslash_escape
  COMMAND use_no_backslash_escape)

set_tests_properties(shacl.cmake.no_backslash_escape PROPERTIES PASS_REGULAR_EXPRESSION "^${expected}$")
