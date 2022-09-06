include(Intel)

file(GENERATE OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/use_intel_fp_model.c
              INPUT ${CMAKE_CURRENT_LIST_DIR}/use_intel_fp_model.c.in)

add_executable(use_intel_fp_model_C ${CMAKE_CURRENT_BINARY_DIR}/use_intel_fp_model.c)
set_target_properties(use_intel_fp_model_C PROPERTIES Intel_FLOATING_POINT_MODEL "consistent")
target_link_libraries(use_intel_fp_model_C PRIVATE shacl::cmake::Intel::FloatingPoint::Model_C)

file(GENERATE OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/use_intel_fp_model.cpp
              INPUT ${CMAKE_CURRENT_LIST_DIR}/use_intel_fp_model.cpp.in
)

add_executable(use_intel_fp_model_CXX ${CMAKE_CURRENT_BINARY_DIR}/use_intel_fp_model.cpp)
set_target_properties(use_intel_fp_model_CXX PROPERTIES Intel_FLOATING_POINT_MODEL "consistent")
target_link_libraries(use_intel_fp_model_CXX PRIVATE shacl::cmake::Intel::FloatingPoint::Model_CXX)

file(GENERATE OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/use_intel_fp_model.F90
              INPUT ${CMAKE_CURRENT_LIST_DIR}/use_intel_fp_model.F90.in
)

add_executable(use_intel_fp_model_Fortran ${CMAKE_CURRENT_BINARY_DIR}/use_intel_fp_model.F90)
set_target_properties(use_intel_fp_model_Fortran PROPERTIES Intel_FLOATING_POINT_MODEL "consistent")
target_link_libraries(use_intel_fp_model_Fortran PRIVATE shacl::cmake::Intel::FloatingPoint::Model_Fortran)

set(expected_C "")
set(expected_CXX "")
set(expected_Fortran "")

if( "${CMAKE_C_COMPILER_ID}" STREQUAL "Intel" )
  set(expected_C "-fp-model=consistent\n")
  if( WIN32 )
    set(expected_C "/fp:consistent\n")
  endif()
endif()

if( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel" )
  set(expected_CXX "-fp-model=consistent\n")
  if( WIN32 )
    set(expected_CXX "/fp:consistent\n")
  endif()
endif()

if( "${CMAKE_Fortran_COMPILER_ID}" STREQUAL "Intel" )
  set(expected_Fortran "-fp-model=consistent\n")
  if( WIN32 )
    set(expected_Fortran "/fp:consistent\n")
  endif()
endif()

add_test(
  NAME shacl.cmake.intel_fp_model_C
  COMMAND use_intel_fp_model_C)
set_tests_properties(shacl.cmake.intel_fp_model_C PROPERTIES PASS_REGULAR_EXPRESSION "^${expected_C}$")

add_test(
  NAME shacl.cmake.intel_fp_model_CXX
  COMMAND use_intel_fp_model_CXX)
set_tests_properties(shacl.cmake.intel_fp_model_CXX PROPERTIES PASS_REGULAR_EXPRESSION "^${expected_CXX}$")

add_test(
  NAME shacl.cmake.intel_fp_model_Fortran
  COMMAND use_intel_fp_model_Fortran)
set_tests_properties(shacl.cmake.intel_fp_model_Fortran PROPERTIES PASS_REGULAR_EXPRESSION "^${expected_Fortran}$")
