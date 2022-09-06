include(Fortran/Backtrace)

set(Intel.Windows.ON "/traceback")
set(Intel.Windows.OFF "/notraceback")
set(Intel.Darwin.ON "-traceback")
set(Intel.Darwin.OFF "-notraceback")
set(Intel.Linux.ON "-traceback")
set(Intel.Linux.OFF "-notraceback")
set(PGI.Windows.ON "-traceback")
set(PGI.Windows.OFF "-notraceback")
set(PGI.Darwin.ON "-traceback")
set(PGI.Darwin.OFF "-notraceback")
set(PGI.Linux.ON "-traceback")
set(PGI.Linux.OFF "-notraceback")
set(GNU.CYGWIN.ON "-fbacktrace")
set(GNU.CYGWIN.OFF "-fno-backtrace")
set(GNU.Windows.ON "-fbacktrace")
set(GNU.Windows.OFF "-fno-backtrace")
set(GNU.Darwin.ON "-fbacktrace")
set(GNU.Darwin.OFF "-fno-backtrace")
set(GNU.Linux.ON "-fbacktrace")
set(GNU.Linux.OFF "-fno-backtrace")

foreach(toggle ON OFF)
  add_executable(backtrace.${toggle} "")
  set_target_properties(backtrace.${toggle}
    PROPERTIES Fortran_BACKTRACE ${toggle})
  target_link_libraries(backtrace.${toggle} PRIVATE shacl::cmake::Fortran::Backtrace)
  set(target backtrace.${toggle})

  configure_file(
    ${CMAKE_CURRENT_LIST_DIR}/backtrace.f90.in
    ${CMAKE_CURRENT_BINARY_DIR}/backtrace.${toggle}.f90.in
    @ONLY)

  file(GENERATE
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/backtrace.${toggle}.f90
    INPUT ${CMAKE_CURRENT_BINARY_DIR}/backtrace.${toggle}.f90.in)

  target_sources(backtrace.${toggle} PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/backtrace.${toggle}.f90)
  add_test(NAME shacl.cmake.Fortran.Backtrace.${toggle} COMMAND backtrace.${toggle})
  set_tests_properties(shacl.cmake.Fortran.Backtrace.${toggle} PROPERTIES
    PASS_REGULAR_EXPRESSION
    "^${${CMAKE_Fortran_COMPILER_ID}.${CMAKE_HOST_SYSTEM_NAME}.${toggle}}\n$")
endforeach()
