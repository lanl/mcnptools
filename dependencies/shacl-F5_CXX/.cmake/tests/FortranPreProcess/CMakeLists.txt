include(FortranPreProcess)

add_executable(fprepro_defines "")
target_compile_definitions(fprepro_defines PRIVATE DO_F_PRE_PRO)

target_sources(fprepro_defines PREPROCESS
  PRIVATE "${CMAKE_CURRENT_LIST_DIR}/fprepro_defines.F90")

add_test(NAME shacl.cmake.FortranPreProcess.Defines
  COMMAND fprepro_defines)

add_executable(fprepro_includes "")
target_include_directories(fprepro_includes PRIVATE ${CMAKE_CURRENT_LIST_DIR})
target_compile_definitions(fprepro_includes PRIVATE DO_F_PRE_PRO)
target_sources(fprepro_includes PREPROCESS PRIVATE
  "${CMAKE_CURRENT_LIST_DIR}/fprepro_includes.F90")

add_test(NAME shacl.cmake.FortranPreProcess.Includes
  COMMAND fprepro_includes)
