string(CONCAT shacl.cmake.Fortran.Backtrace.generator
  "$<$<STREQUAL:PGI,${CMAKE_Fortran_COMPILER_ID}>:"
    "$<IF:$<BOOL:$<TARGET_PROPERTY:Fortran_BACKTRACE>>"
        ",-traceback"
        ",-notraceback>>")

target_compile_options(shacl::cmake::Fortran::Backtrace INTERFACE
  ${shacl.cmake.Fortran.Backtrace.generator})

unset(shacl.cmake.Fortran.Backtrace.generator)
