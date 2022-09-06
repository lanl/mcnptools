string(CONCAT shacl.cmake.Fortran.StandardCompliance.generator
  "$<$<STREQUAL:GNU,${CMAKE_Fortran_COMPILER_ID}>:-pedantic-errors>")

target_compile_options(shacl::cmake::Fortran::StandardCompliance INTERFACE
  ${shacl.cmake.Fortran.StandardCompliance.generator})

unset(shacl.cmake.Fortran.StandardCompliance.generator)
