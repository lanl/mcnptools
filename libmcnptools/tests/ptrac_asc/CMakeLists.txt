add_library(ptrac_asc_tests INTERFACE)
target_include_directories(ptrac_asc_tests INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

add_subdirectory(tests)

configure_file(../test_files/ptrac_asc ./tests/test_ptrac COPYONLY)
configure_file(../test_files/ptrac_asc_prdmp_neg_1 ./tests/test_ptrac_prdmp_neg_1 COPYONLY)
