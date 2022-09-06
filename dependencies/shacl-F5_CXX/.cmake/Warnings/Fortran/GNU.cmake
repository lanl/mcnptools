string(CONCAT shacl.cmake.Warnings.generator
  "$<$<BOOL:$<TARGET_PROPERTY:WARN_ERROR>>:-Werror;>"
  "$<$<BOOL:$<TARGET_PROPERTY:WARN_ALL>>:"
    "-Wall;"
    "-Wextra;"
    "-Wpedantic;"
    "-Wcharacter-truncation;"
    "-Wrealloc-lhs;"
    "$<$<VERSION_LESS:${CMAKE_Fortran_COMPILER_VERSION},7.3>:-Wno-surprising;>"
    "-Wuse-without-only;>"
  "$<$<BOOL:$<TARGET_PROPERTY:GNU_ENABLED_WARNINGS>>:"
   "-W$<JOIN:$<TARGET_PROPERTY:GNU_ENABLED_WARNINGS>,;-W>;>"
  "$<$<BOOL:$<TARGET_PROPERTY:GNU_DISABLED_WARNINGS>>:"
   "-Wno-$<JOIN:$<TARGET_PROPERTY:GNU_DISABLED_WARNINGS>,;-Wno->;>")

target_compile_options(shacl::cmake::Warnings_Fortran INTERFACE
  $<$<STREQUAL:${CMAKE_Fortran_COMPILER_ID},GNU>:${shacl.cmake.Warnings.generator}>)

unset(shacl.cmake.Warnings.generator)
