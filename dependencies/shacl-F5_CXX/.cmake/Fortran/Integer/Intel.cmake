if(NOT DEFINED CMAKE_Fortran_COMPILER)
  return()
endif()

include_guard(GLOBAL)
string(CONCAT shacl.cmake.Fortran.Integer.generator
  "$<$<STREQUAL:${CMAKE_Fortran_COMPILER_ID},Intel>:"
    "$<$<NOT:$<PLATFORM_ID:Windows>>:"
      "$<$<EQUAL:$<TARGET_PROPERTY:Fortran_INTEGER_SIZE_BYTES>,8>:-i8>>"
    "$<$<PLATFORM_ID:Windows>:"
      "$<$<EQUAL:$<TARGET_PROPERTY:Fortran_INTEGER_SIZE_BYTES>,4>:"
        "/integer-size$<1::>32>"
      "$<$<EQUAL:$<TARGET_PROPERTY:Fortran_INTEGER_SIZE_BYTES>,8>:"
        "/integer-size$<1::>64>>>")

target_compile_options(shacl::cmake::Fortran::Integer_Fortran INTERFACE
  ${shacl.cmake.Fortran.Integer.generator})

string(CONCAT shacl.cmake.Fortran.Integer.generator
  "$<$<STREQUAL:${CMAKE_Fortran_COMPILER_ID},Intel>:"
    "$<$<NOT:$<BOOL:$<TARGET_PROPERTY:Fortran_INTEGER_SIZE_BYTES>>>:"
      "F90_INT_4BYTE>>")

target_compile_definitions(shacl::cmake::Fortran::Integer::detail INTERFACE
  ${shacl.cmake.Fortran.Integer.generator})

unset(shacl.cmake.Fortran.Integer.generator)

