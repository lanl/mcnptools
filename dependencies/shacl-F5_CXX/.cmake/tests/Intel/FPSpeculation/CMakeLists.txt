include(Intel)

file(GENERATE OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/use_intel_fp_speculation.c
              INPUT ${CMAKE_CURRENT_LIST_DIR}/use_intel_fp_speculation.c.in)

add_executable(use_intel_fp_speculation_C ${CMAKE_CURRENT_BINARY_DIR}/use_intel_fp_speculation.c)
set_target_properties(use_intel_fp_speculation_C PROPERTIES Intel_FLOATING_POINT_SPECULATION "safe")
target_link_libraries(use_intel_fp_speculation_C PRIVATE shacl::cmake::Intel::FloatingPoint::Speculation_C)

file(GENERATE OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/use_intel_fp_speculation.cpp
              INPUT ${CMAKE_CURRENT_LIST_DIR}/use_intel_fp_speculation.cpp.in)

add_executable(use_intel_fp_speculation_CXX ${CMAKE_CURRENT_BINARY_DIR}/use_intel_fp_speculation.cpp)
set_target_properties(use_intel_fp_speculation_CXX PROPERTIES Intel_FLOATING_POINT_SPECULATION "safe")
target_link_libraries(use_intel_fp_speculation_CXX PRIVATE shacl::cmake::Intel::FloatingPoint::Speculation_CXX)

file(GENERATE OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/use_intel_fp_speculation.F90
              INPUT ${CMAKE_CURRENT_LIST_DIR}/use_intel_fp_speculation.F90.in)

add_executable(use_intel_fp_speculation_Fortran ${CMAKE_CURRENT_BINARY_DIR}/use_intel_fp_speculation.F90)
set_target_properties(use_intel_fp_speculation_Fortran PROPERTIES Intel_FLOATING_POINT_SPECULATION "safe")
target_link_libraries(use_intel_fp_speculation_Fortran PRIVATE shacl::cmake::Intel::FloatingPoint::Speculation_Fortran)

set(expected_C "")
set(expected_CXX "")
set(expected_Fortran "")

if( "${CMAKE_C_COMPILER_ID}" STREQUAL "Intel" )
  set(expected_C "-fp-speculation=safe\n")
  if( WIN32 )
    set(expected_C "/Qfp-speculation:safe\n")
  endif()
endif()

if( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel" )
  set(expected_CXX "-fp-speculation=safe\n")
  if( WIN32 )
    set(expected_CXX "/Qfp-speculation:safe\n")
  endif()
endif()

if( "${CMAKE_Fortran_COMPILER_ID}" STREQUAL "Intel" )
  set(expected_Fortran "-fp-speculation=safe\n")
  if( WIN32 )
    set(expected_Fortran "/Qfp-speculation:safe\n")
  endif()
endif()

add_test(
  NAME shacl.cmake.intel_fp_speculation_C
  COMMAND use_intel_fp_speculation_C
)
set_tests_properties(shacl.cmake.intel_fp_speculation_C PROPERTIES PASS_REGULAR_EXPRESSION "^${expected_C}$")

add_test(
  NAME shacl.cmake.intel_fp_speculation_CXX
  COMMAND use_intel_fp_speculation_CXX
)
set_tests_properties(shacl.cmake.intel_fp_speculation_CXX PROPERTIES PASS_REGULAR_EXPRESSION "^${expected_CXX}$")

add_test(
  NAME shacl.cmake.intel_fp_speculation_Fortran
  COMMAND use_intel_fp_speculation_Fortran
)
set_tests_properties(shacl.cmake.intel_fp_speculation_Fortran PROPERTIES PASS_REGULAR_EXPRESSION "^${expected_Fortran}$")
