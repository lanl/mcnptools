string(CONCAT shacl.cmake.Fortran.LongLines.generator
  "$<$<STREQUAL:GNU,${CMAKE_Fortran_COMPILER_ID}>:"
    "-ffree-line-length-none;>")

target_compile_options(shacl::cmake::Fortran::LongLines INTERFACE
  ${shacl.cmake.Fortran.LongLines.generator})

unset(shacl.cmake.Fortran.LongLines.generator)
