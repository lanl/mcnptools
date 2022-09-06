include(Fortran)

foreach(size 4 8)
  add_executable(Fortran.Integer${size}.C.test test.c)
  set_target_properties(Fortran.Integer${size}.C.test PROPERTIES Fortran_INTEGER_SIZE_BYTES "${size}")
  target_link_libraries(Fortran.Integer${size}.C.test PUBLIC shacl::cmake::Fortran::Integer_C)
  target_compile_definitions(Fortran.Integer${size}.C.test PRIVATE EXPECTED=${size})
  add_test(NAME shacl.cmake.Fortran.Integer${size}.C.test COMMAND Fortran.Integer${size}.C.test)

  add_executable(Fortran.Integer${size}.CXX.test test.cpp)
  set_target_properties(Fortran.Integer${size}.CXX.test PROPERTIES Fortran_INTEGER_SIZE_BYTES "${size}")
  target_link_libraries(Fortran.Integer${size}.CXX.test PUBLIC shacl::cmake::Fortran::Integer_CXX)
  target_compile_definitions(Fortran.Integer${size}.CXX.test PRIVATE EXPECTED=${size})
  add_test(NAME shacl.cmake.Fortran.Integer${size}.CXX.test COMMAND Fortran.Integer${size}.CXX.test)

  add_executable(Fortran.Integer${size}.Fortran.test test.F90)
  set_target_properties(Fortran.Integer${size}.Fortran.test PROPERTIES Fortran_STANDARD 2008)
  set_target_properties(Fortran.Integer${size}.Fortran.test PROPERTIES Fortran_INTEGER_SIZE_BYTES "${size}")
  target_link_libraries(Fortran.Integer${size}.Fortran.test PUBLIC shacl::cmake::Fortran::Integer_Fortran)
  add_test(NAME shacl.cmake.Fortran.Integer${size}.Fortran.test COMMAND Fortran.Integer${size}.Fortran.test)
endforeach()


