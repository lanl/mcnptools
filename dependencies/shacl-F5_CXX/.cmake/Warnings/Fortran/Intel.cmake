string(CONCAT shacl.cmake.Warnings.generator
  "$<IF:$<PLATFORM_ID:Windows>"
      ",/warn$<1::>"
      ",-warn;>")

string(CONCAT shacl.cmake.Warnings.generator
  "$<$<BOOL:$<TARGET_PROPERTY:WARN_ERROR>>:"
    "${shacl.cmake.Warnings.generator}error$<COMMA>stderror>"
  "$<$<BOOL:$<TARGET_PROPERTY:WARN_ALL>>:"
    "$<IF:$<BOOL:$<TARGET_PROPERTY:WARN_ERROR>>"
        ",$<COMMA>"
        ",${shacl.cmake.Warnings.generator}>"
    "all>;"
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

target_compile_options(shacl::cmake::Warnings_Fortran INTERFACE
  $<$<STREQUAL:${CMAKE_Fortran_COMPILER_ID},Intel>:${shacl.cmake.Warnings.generator}>)

unset(shacl.cmake.Warnings.generator)
