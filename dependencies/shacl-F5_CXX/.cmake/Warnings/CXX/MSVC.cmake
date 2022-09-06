string(CONCAT shacl.cmake.Warnings.generator
  "$<$<BOOL:$<TARGET_PROPERTY:WARN_ERROR>>:/WX;>"
  "$<$<BOOL:$<TARGET_PROPERTY:WARN_ALL>>:/W4;>"
  "$<$<BOOL:$<TARGET_PROPERTY:MSVC_ENABLED_WARNINGS>>:"
    "$<$<NOT:$<BOOL:$<TARGET_PROPERTY:WARN_ALL>>>:/W1;>"
    "/w1$<JOIN:$<TARGET_PROPERTY:MSVC_ENABLED_WARNINGS>"
             ",;/w1>;>"
  "$<$<BOOL:$<TARGET_PROPERTY:MSVC_DISABLED_WARNINGS>>:"
    "/wd$<JOIN:$<TARGET_PROPERTY:MSVC_DISABLED_WARNINGS>"
             ",;/wd>;>")

target_compile_options(shacl::cmake::Warnings_CXX INTERFACE
  $<$<CXX_COMPILER_ID:MSVC>:${shacl.cmake.Warnings.generator}>)

unset(shacl.cmake.Warnings.generator)
