add_executable(shacl.cmake.Fortran.Exceptions a.f90)
set_target_properties(shacl.cmake.Fortran.Exceptions PROPERTIES
  LINKER_LANGUAGE Fortran)
add_library(shacl.cmake.Fortran.Exceptions.lib STATIC b.cpp)

target_link_libraries(shacl.cmake.Fortran.Exceptions PRIVATE
  shacl.cmake.Fortran.Exceptions.lib
  shacl::cmake::Fortran::Exceptions)

add_test(
  NAME shacl.cmake.Fortran.Exceptions
  COMMAND shacl.cmake.Fortran.Exceptions)
