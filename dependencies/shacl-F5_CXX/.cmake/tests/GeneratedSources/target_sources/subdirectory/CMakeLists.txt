add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/foo.c"
  COMMAND "${CMAKE_COMMAND}" -E copy
          "${CMAKE_CURRENT_SOURCE_DIR}/foo.c.in"
          "${CMAKE_CURRENT_BINARY_DIR}/foo.c")

target_sources(GeneratedSources.library GENERATED
  PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/foo.c")
