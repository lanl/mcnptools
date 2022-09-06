string(CONCAT shacl.cmake.Fortran.Exceptions.generator
  "$<$<AND:$<STREQUAL:$<TARGET_PROPERTY:LINKER_LANGUAGE>,Fortran>"
         ",$<PLATFORM_ID:Darwin>"
         ",$<STREQUAL:GNU,${CMAKE_Fortran_COMPILER_ID}>>:"
    "-Wl,-keep_dwarf_unwind>")

target_link_libraries(shacl::cmake::Fortran::Exceptions INTERFACE
  ${shacl.cmake.Fortran.Exceptions.generator})

unset(shacl.cmake.Fortran.Exceptions.generator)
