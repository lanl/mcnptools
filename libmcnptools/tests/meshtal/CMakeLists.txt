add_library(meshtal_tests INTERFACE)
target_include_directories(meshtal_tests INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

add_subdirectory(tests)

configure_file(../test_files/meshtal ./tests/test_meshtal COPYONLY)
