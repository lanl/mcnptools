include(Fortran)

file(GENERATE OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/use_temp_arrays.F90"
              INPUT "${CMAKE_CURRENT_LIST_DIR}/use_temp_arrays.F90.in")

add_executable(use_temp_arrays "${CMAKE_CURRENT_BINARY_DIR}/use_temp_arrays.F90")
set_target_properties(use_temp_arrays PROPERTIES Fortran_STACK_ARRAY_SIZE_LIMIT 1024)
target_link_libraries(use_temp_arrays PRIVATE shacl::cmake::Fortran::StackArraySizeLimit)

set(expected_GNU "-fmax-stack-var-size=1024000")
if( WIN32 )
  set(expected_Intel "/heap-arrays:1024")
else()
  set(expected_Intel "-heap-arrays 1024")
endif()
set(expected_PGI "-Mnostack_arrays")

add_test(
  NAME shacl.cmake.Fortran.StackArraySizeLimit
  COMMAND use_temp_arrays)

set_tests_properties(shacl.cmake.Fortran.StackArraySizeLimit PROPERTIES
  PASS_REGULAR_EXPRESSION "^${expected_${CMAKE_Fortran_COMPILER_ID}}\n$")
