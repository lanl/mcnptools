string(CONCAT shacl.cmake.Warnings.generator
  "$<$<BOOL:$<TARGET_PROPERTY:WARN_ERROR>>:-Werror>;"
  "$<$<BOOL:$<TARGET_PROPERTY:WARN_ALL>>:"
    "-Wall;"
    "-Wextra;"
    "-Wpedantic;"
    "$<$<VERSION_GREATER:$<C_COMPILER_VERSION>,7.0>:-Wduplicated-branches;>"
    "$<$<VERSION_GREATER:$<C_COMPILER_VERSION>,6.0>:-Wduplicated-cond;>>"
  "$<$<BOOL:$<TARGET_PROPERTY:GNU_ENABLED_WARNINGS>>:"
    "-W$<JOIN:$<TARGET_PROPERTY:GNU_ENABLED_WARNINGS>"
            ",;-W>;>"
  "$<$<BOOL:$<TARGET_PROPERTY:GNU_DISABLED_WARNINGS>>:"
    "-Wno-$<JOIN:$<TARGET_PROPERTY:GNU_DISABLED_WARNINGS>"
               ",;-Wno->;>")

target_compile_options(shacl::cmake::Warnings_C INTERFACE
  $<$<C_COMPILER_ID:GNU>:${shacl.cmake.Warnings.generator}>)

unset(shacl.cmake.Warnings.generator)
