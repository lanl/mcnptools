string(CONCAT shacl.cmake.Fortran.BackslashEscape.generator
  "$<$<STREQUAL:GNU,${CMAKE_Fortran_COMPILER_ID}>:"
    "$<$<BOOL:$<TARGET_PROPERTY:Fortran_BACKSLASH_ESCAPE>>:-fbackslash>>")

target_compile_options(shacl::cmake::Fortran::BackslashEscape INTERFACE
  ${shacl.cmake.Fortran.BackslashEscape.generator})

unset(shacl.cmake.Fortran.BackslashEscape.generator)
