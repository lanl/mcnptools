include(Fortran)

set(module_dir "${CMAKE_CURRENT_BINARY_DIR}/fortran_modules")

add_library(testlib STATIC "test_mod.F90")
target_Fortran_module_directory(testlib PRIVATE
  BUILD_INTERFACE "${module_dir}")

set(test_module_dir "${module_dir}")

set(test_module_dir_suffix "")
if( CMAKE_CONFIGURATION_TYPES )
  set(test_module_dir_suffix "/$<CONFIG>")
endif()

add_test(
  NAME shacl.cmake.Fortran.ModuleDirectory
  COMMAND
    ${CMAKE_COMMAND}
    -D module_file=${test_module_dir}${test_module_dir_suffix}/test_mod.mod
    -P "${CMAKE_CURRENT_LIST_DIR}/check_exists.cmake")
