string(CONCAT shacl.cmake.Fortran.BackslashEscape.generator
  "$<$<STREQUAL:PGI,${CMAKE_Fortran_COMPILER_ID}>:"
    "$<IF:$<BOOL:$<TARGET_PROPERTY:Fortran_BACKSLASH_ESCAPE>>"
        ",-Mbackslash"
        ",-Mnobackslash>>")

target_compile_options(shacl::cmake::Fortran::BackslashEscape INTERFACE
  ${shacl.cmake.Fortran.BackslashEscape.generator})

unset(shacl.cmake.Fortran.BackslashEscape.generator)
