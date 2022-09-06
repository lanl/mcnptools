string(CONCAT shacl.cmake.Fortran.Standard.generator
  "$<$<STREQUAL:$<TARGET_PROPERTY:Fortran_STANDARD>,95>:f95>"
  "$<$<STREQUAL:$<TARGET_PROPERTY:Fortran_STANDARD>,2003>:f03>"
  "$<$<STREQUAL:$<TARGET_PROPERTY:Fortran_STANDARD>,2008>:f08>"
  "$<$<STREQUAL:$<TARGET_PROPERTY:Fortran_STANDARD>,2018>:"
    "$<IF:$<VERSION_LESS:${CMAKE_Fortran_COMPILER_VERSION},19>,f15,f18>>")

string(CONCAT shacl.cmake.Fortran.Standard.generator
  "$<$<STREQUAL:Intel,${CMAKE_Fortran_COMPILER_ID}>:"
    "$<IF:$<PLATFORM_ID:Windows>"
        ",/stand$<1::>"
        ",-stand=>"
    "${shacl.cmake.Fortran.Standard.generator}>")

target_compile_options(shacl::cmake::Fortran::Standard INTERFACE
  ${shacl.cmake.Fortran.Standard.generator})

unset(shacl.cmake.Fortran.Standard.generator)
