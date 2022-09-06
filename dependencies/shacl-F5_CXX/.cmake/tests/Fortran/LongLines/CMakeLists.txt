include(Fortran/LongLines)

set(GNU.CYGWIN "-ffree-line-length-none")
set(GNU.Windows "-ffree-line-length-none")
set(GNU.Darwin "-ffree-line-length-none")
set(GNU.Linux "-ffree-line-length-none")

add_executable(longlines)
target_link_libraries(longlines PRIVATE shacl::cmake::Fortran::LongLines)

file(GENERATE
  OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/longlines.f90"
  INPUT "${CMAKE_CURRENT_LIST_DIR}/longlines.f90.in")

target_sources(longlines PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/longlines.f90")
add_test(NAME shacl.cmake.Fortran.LongLines COMMAND longlines)
set_tests_properties(shacl.cmake.Fortran.LongLines PROPERTIES
  PASS_REGULAR_EXPRESSION
  "^${${CMAKE_Fortran_COMPILER_ID}.${CMAKE_HOST_SYSTEM_NAME}}\n$")
