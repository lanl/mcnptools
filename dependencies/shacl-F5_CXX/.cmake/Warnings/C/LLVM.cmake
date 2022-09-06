string(CONCAT shacl.cmake.Warnings.generator
  "$<$<BOOL:$<TARGET_PROPERTY:WARN_ERROR>>:-Werror>;"
  "$<$<BOOL:$<TARGET_PROPERTY:WARN_ALL>>:"
    "-Wall;"
    "-Wextra;"
    "-Wpedantic;"
    "-Wassign-enum;"
    "-Wbad-function-cast;"
    "-Wkeyword-macro;"
    "-Wnonportable-system-include-path;"
    "-Wsometimes-uninitialized;>"
  "$<$<BOOL:$<TARGET_PROPERTY:LLVM_ENABLED_WARNINGS>>:"
    "-W$<JOIN:$<TARGET_PROPERTY:LLVM_ENABLED_WARNINGS>"
            ",;-W>;>"
  "$<$<BOOL:$<TARGET_PROPERTY:LLVM_DISABLED_WARNINGS>>:"
    "-Wno-$<JOIN:$<TARGET_PROPERTY:LLVM_DISABLED_WARNINGS>"
               ",;-Wno->;>")

target_compile_options(shacl::cmake::Warnings_C INTERFACE
  $<$<OR:$<C_COMPILER_ID:AppleClang>,$<C_COMPILER_ID:Clang>>:${shacl.cmake.Warnings.generator}>)

unset(shacl.cmake.Warnings.generator)
