include(Fortran)

foreach( size 4 8 )
  add_executable(Fortran.Real${size}.C.test test.c)
  set_target_properties(Fortran.Real${size}.C.test PROPERTIES Fortran_REAL_SIZE_BYTES "${size}")
  target_link_libraries(Fortran.Real${size}.C.test PUBLIC shacl::cmake::Fortran::Real_C)
  target_compile_definitions(Fortran.Real${size}.C.test PRIVATE EXPECTED=${size})
  add_test(NAME shacl.cmake.Fortran.Real${size}.C.test COMMAND Fortran.Real${size}.C.test)

  add_executable(Fortran.Real${size}.CXX.test test.cpp)
  set_target_properties(Fortran.Real${size}.CXX.test PROPERTIES Fortran_REAL_SIZE_BYTES "${size}")
  target_link_libraries(Fortran.Real${size}.CXX.test PUBLIC shacl::cmake::Fortran::Real_CXX)
  target_compile_definitions(Fortran.Real${size}.CXX.test PRIVATE EXPECTED=${size})
  add_test(NAME shacl.cmake.Fortran.Real${size}.CXX.test COMMAND Fortran.Real${size}.CXX.test)

  add_executable(Fortran.Real${size}.Fortran.test test.F90)
  set_target_properties(Fortran.Real${size}.Fortran.test PROPERTIES Fortran_REAL_SIZE_BYTES "${size}")
  set_target_properties(Fortran.Real${size}.Fortran.test PROPERTIES Fortran_STANDARD 2008)
  target_link_libraries(Fortran.Real${size}.Fortran.test PUBLIC  shacl::cmake::Fortran::Real_Fortran)
  add_test(NAME shacl.cmake.Fortran.Real${size}.Fortran.test COMMAND Fortran.Real${size}.Fortran.test)
endforeach()


