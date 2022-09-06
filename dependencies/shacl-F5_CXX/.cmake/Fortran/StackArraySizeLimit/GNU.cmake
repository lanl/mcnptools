string(CONCAT shacl.cmake.Fortran.StackArraySizeLimit.generator
  "$<$<STREQUAL:GNU,${CMAKE_Fortran_COMPILER_ID}>:"
    "$<$<BOOL:$<TARGET_PROPERTY:Fortran_STACK_ARRAY_SIZE_LIMIT>>:"
      "-fmax-stack-var-size="
      "$<TARGET_PROPERTY:Fortran_STACK_ARRAY_SIZE_LIMIT>000>>")

target_compile_options(shacl::cmake::Fortran::StackArraySizeLimit INTERFACE
  ${shacl.cmake.Fortran.StackArraySizeLimit.generator})

unset(shacl.cmake.Fortran.StackArraySizeLimit.generator)
