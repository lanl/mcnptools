string(CONCAT shacl.cmake.Fortran.Backtrace.generator
  "$<$<STREQUAL:Intel,${CMAKE_Fortran_COMPILER_ID}>:"
    "$<IF:$<BOOL:$<TARGET_PROPERTY:Fortran_BACKTRACE>>"
        ",$<IF:$<PLATFORM_ID:Windows>"
             ",/traceback"
             ",-traceback>"
        ",$<IF:$<PLATFORM_ID:Windows>"
             ",/notraceback"
             ",-notraceback>>>")

target_compile_options(shacl::cmake::Fortran::Backtrace INTERFACE
  ${shacl.cmake.Fortran.Backtrace.generator})

unset(shacl.cmake.Fortran.Backtrace.generator)
