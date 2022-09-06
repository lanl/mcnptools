include(GeneratedSources)

add_library(GeneratedSources.library STATIC)

add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/bar.c"
  COMMAND "${CMAKE_COMMAND}" -E copy
          "${CMAKE_CURRENT_SOURCE_DIR}/bar.c.in"
          "${CMAKE_CURRENT_BINARY_DIR}/bar.c")

add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/foo.c"
  COMMAND "${CMAKE_COMMAND}" -E copy
          "${CMAKE_CURRENT_SOURCE_DIR}/foo.c.in"
          "${CMAKE_CURRENT_BINARY_DIR}/foo.c"
		  
		  )

target_sources(GeneratedSources.library GENERATED
  PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/foo.c"
  PUBLIC  "${CMAKE_CURRENT_BINARY_DIR}/bar.c")

add_subdirectory(subdirectory)

add_test(
  NAME
    shacl.cmake.target_sources1
  COMMAND
    "${CMAKE_COMMAND}" -E compare_files
    "${CMAKE_CURRENT_SOURCE_DIR}/bar.c.in"
    "${CMAKE_CURRENT_BINARY_DIR}/bar.c")
