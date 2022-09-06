include(Fortran)

file(GENERATE
  OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/use_fortran_standard_adherence.f90"
  INPUT "${CMAKE_CURRENT_LIST_DIR}/use_fortran_standard_adherence.F90.in")

add_executable(fortran_standard_adherence "${CMAKE_CURRENT_BINARY_DIR}/use_fortran_standard_adherence.f90")
target_link_libraries(fortran_standard_adherence PUBLIC shacl::cmake::Fortran::StandardCompliance)

set(expected_PGI "-Mstandard -Mnobackslash")
if( WIN32 )
  set(expected_Intel "/standard-semantics /assume:nostd_mod_proc_name /assume:nobscc")
else()
  set(expected_Intel "-standard-semantics SHELL:-assume nostd_mod_proc_name SHELL:-assume nobscc")
endif()
set(expected_GNU "-pedantic-errors")
set(expected "${expected_${CMAKE_Fortran_COMPILER_ID}}\n")

add_test(NAME shacl.cmake.Fortran.StandardAdherence
  COMMAND fortran_standard_adherence)

set_tests_properties(shacl.cmake.Fortran.StandardAdherence PROPERTIES
  PASS_REGULAR_EXPRESSION "^${expected}$")
