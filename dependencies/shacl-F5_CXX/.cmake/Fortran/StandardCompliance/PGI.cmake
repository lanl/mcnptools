string(CONCAT shacl.cmake.Fortran.StandardCompliance.generator
  "$<$<STREQUAL:PGI,${CMAKE_Fortran_COMPILER_ID}>:-Mstandard>")

target_compile_options(shacl::cmake::Fortran::StandardCompliance INTERFACE
  ${shacl.cmake.Fortran.StandardCompliance.generator})

unset(shacl.cmake.Fortran.StandardCompliance.generator)
