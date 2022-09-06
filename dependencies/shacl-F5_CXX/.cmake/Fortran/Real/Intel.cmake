if(NOT DEFINED CMAKE_Fortran_COMPILER)
  return()
endif()

include_guard(GLOBAL)
string(CONCAT shacl.cmake.Fortran.Real.generator
  "$<$<STREQUAL:${CMAKE_Fortran_COMPILER_ID},Intel>:"
    "$<$<NOT:$<PLATFORM_ID:Windows>>:"
      "$<$<STREQUAL:$<TARGET_PROPERTY:Fortran_REAL_SIZE_BYTES>,8>:-r8>>"
    "$<$<PLATFORM_ID:Windows>:"
      "$<$<STREQUAL:$<TARGET_PROPERTY:Fortran_REAL_SIZE_BYTES>,4>:"
        "/real-size$<1::>32>"
    "$<$<STREQUAL:$<TARGET_PROPERTY:Fortran_REAL_SIZE_BYTES>,8>:"
        "/real-size$<1::>64>>>")

target_compile_options(shacl::cmake::Fortran::Real_Fortran INTERFACE
  ${shacl.cmake.Fortran.Real.generator})

string(CONCAT shacl.cmake.Fortran.Real.generator
  "$<$<STREQUAL:${CMAKE_Fortran_COMPILER_ID},Intel>:"
    "$<$<NOT:$<BOOL:$<TARGET_PROPERTY:Fortran_REAL_SIZE_BYTES>>>:"
      "F90_REAL_4BYTE>>")

target_compile_definitions(shacl::cmake::Fortran::Real::detail INTERFACE
  ${shacl.cmake.Fortran.Real.generator})

unset(shacl.cmake.Fortran.Real.generator)
