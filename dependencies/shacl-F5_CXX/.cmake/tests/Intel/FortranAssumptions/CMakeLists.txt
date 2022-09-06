include(Intel/Fortran/Assumptions)

add_executable(assumptions.test "")
set_target_properties(assumptions.test PROPERTIES
  Intel_Fortran_ENABLED_ASSUMPTIONS "old_xor;std_intent_in"
  Intel_Fortran_DISABLED_ASSUMPTIONS "protect_parens;realloc_lhs")
target_link_libraries(assumptions.test PUBLIC shacl::cmake::Intel::Fortran::Assumptions)

file(GENERATE
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/assumptions.f90
  INPUT ${CMAKE_CURRENT_LIST_DIR}/assumptions.f90.in)

target_sources(assumptions.test PUBLIC
  ${CMAKE_CURRENT_BINARY_DIR}/assumptions.f90)

add_test(NAME shacl.cmake.Intel.FortranAssumptions COMMAND assumptions.test)

if(CMAKE_Fortran_COMPILER_ID STREQUAL "Intel")
  if(WIN32)
    set(regex "/assume:")
  else()
    set(regex "-assume;")
  endif()
  set(regex "${regex}old_xor,std_intent_in,noprotect_parens,norealloc_lhs")
endif()

set_tests_properties(shacl.cmake.Intel.FortranAssumptions
  PROPERTIES PASS_REGULAR_EXPRESSION "${regex}")
