string(CONCAT shacl.cmake.Fortran.StandardCompliance.generator
  "$<$<STREQUAL:Intel,${CMAKE_Fortran_COMPILER_ID}>:"
    "$<IF:$<PLATFORM_ID:Windows>"
        ",/standard-semantics"
        ";/assume$<1::>nostd_mod_proc_name"
        ",-standard-semantics"
        ";SHELL:-assume nostd_mod_proc_name>>")

target_compile_options(shacl::cmake::Fortran::StandardCompliance
  INTERFACE ${shacl.cmake.Fortran.StandardCompliance.generator})

unset(shacl.cmake.Fortran.StandardCompliance.generator)
