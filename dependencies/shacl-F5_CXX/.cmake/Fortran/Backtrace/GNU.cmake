string(CONCAT shacl.cmake.Fortran.Backtrace.generator
  "$<$<STREQUAL:GNU,${CMAKE_Fortran_COMPILER_ID}>:"
    "$<IF:$<BOOL:$<TARGET_PROPERTY:Fortran_BACKTRACE>>"
        ",-fbacktrace"
        ",-fno-backtrace>>")

target_compile_options(shacl::cmake::Fortran::Backtrace INTERFACE
  ${shacl.cmake.Fortran.Backtrace.generator})

unset(shacl.cmake.Fortran.Backtrace.generator)
