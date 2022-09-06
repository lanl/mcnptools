string(CONCAT shacl.cmake.Warnings.generator
  "$<$<BOOL:$<TARGET_PROPERTY:WARN_ERROR>>:"
    "$<IF:$<PLATFORM_ID:Windows>"
        ",/WX"
        ",-Werror>;>"
  "$<$<BOOL:$<TARGET_PROPERTY:WARN_ALL>>:"
    "$<IF:$<PLATFORM_ID:Windows>"
        ",/W3"
        ",-w2>;>"
  "$<$<BOOL:$<TARGET_PROPERTY:Intel_ENABLED_WARNINGS>>:"
    "$<IF:$<PLATFORM_ID:Windows>"
        ",/Qdiag-enable$<1::>"
        ",-diag-enable=>"
    "$<JOIN:$<TARGET_PROPERTY:Intel_ENABLED_WARNINGS>,$<COMMA>>;>"
  "$<$<BOOL:$<TARGET_PROPERTY:Intel_DISABLED_WARNINGS>>:"
    "$<IF:$<PLATFORM_ID:Windows>"
        ",/Qdiag-disable$<1::>"
        ",-diag-disable=>"
    "$<JOIN:$<TARGET_PROPERTY:Intel_DISABLED_WARNINGS>,$<COMMA>>;>")

target_compile_options(shacl::cmake::Warnings_C INTERFACE
  $<$<C_COMPILER_ID:Intel>:${shacl.cmake.Warnings.generator}>)

unset(shacl.cmake.Warnings.generator)
