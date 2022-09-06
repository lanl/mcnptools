add_library(ptrac_bin_tests INTERFACE)
target_include_directories(ptrac_bin_tests INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

add_subdirectory(tests)

configure_file(../test_files/ptrac_bin ./tests/test_ptrac COPYONLY)
