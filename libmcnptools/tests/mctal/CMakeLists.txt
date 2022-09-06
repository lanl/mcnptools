add_library(mctal_tests INTERFACE)
target_include_directories(mctal_tests INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

add_subdirectory(tests)

configure_file(../test_files/mctal ./tests/test_mctal COPYONLY)
configure_file(../test_files/mctal_prdmp_neg_1 ./tests/test_mctal_prdmp_neg_1 COPYONLY)
configure_file(../test_files/mctal_malformed ./tests/test_mctal_malformed COPYONLY)
