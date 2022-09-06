string(CONCAT shacl.cmake.Fortran.BackslashEscape.generator
  "$<$<STREQUAL:Intel,${CMAKE_Fortran_COMPILER_ID}>:"
    "$<IF:$<BOOL:$<TARGET_PROPERTY:Fortran_BACKSLASH_ESCAPE>>"
        ",$<IF:$<PLATFORM_ID:Windows>"
             ",/assume$<1::>bscc"
             ",SHELL:-assume bscc>"
        ",$<IF:$<PLATFORM_ID:Windows>"
             ",/assume$<1::>nobscc"
             ",SHELL:-assume nobscc>>>")

target_compile_options(shacl::cmake::Fortran::BackslashEscape INTERFACE
  ${shacl.cmake.Fortran.BackslashEscape.generator})

unset(shacl.cmake.Fortran.BackslashEscape.generator)
