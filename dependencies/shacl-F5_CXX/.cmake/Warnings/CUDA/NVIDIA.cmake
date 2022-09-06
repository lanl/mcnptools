string(CONCAT shacl.cmake.Warnings.generator
  "-Xcompiler;"
  "$<JOIN:$<GENEX_EVAL:$<TARGET_PROPERTY:Warnings_CXX"
                                       ",INTERFACE_COMPILE_OPTIONS>>"
        ",$<COMMA>>"
  # We're assuming at least one flag is passed by the GENEX_EVAL
  "$<$<CXX_COMPILER_ID:GNU>:$<COMMA>-Wno-pedantic>")

target_compile_options(shacl::cmake::Warnings_CUDA INTERFACE
  $<$<STREQUAL:${CMAKE_CUDA_COMPILER_ID},NVIDIA>:${shacl.cmake.Warnings.generator}>)

unset(shacl.cmake.Warnings.generator)
