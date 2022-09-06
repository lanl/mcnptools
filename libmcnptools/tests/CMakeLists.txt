# Google Test libraries.
add_subdirectory(gtest)
add_library(mcnptools_gtest INTERFACE)
target_include_directories(mcnptools_gtest INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/gtest/include>
)
target_link_libraries(mcnptools_gtest INTERFACE gtest_main gtest)

MACRO(MakeTest SUITE TEST_SRC)
  GET_FILENAME_COMPONENT(TESTNAME ${TEST_SRC} NAME_WE)

  ADD_EXECUTABLE(${TESTNAME} ${TEST_SRC})
  TARGET_LINK_LIBRARIES(${TESTNAME} PRIVATE mcnptools)
  TARGET_LINK_LIBRARIES(${TESTNAME} PRIVATE mcnptools_gtest)
  TARGET_LINK_LIBRARIES(${TESTNAME} PRIVATE ${ARGN})

  ADD_TEST(NAME ${TESTNAME} COMMAND ${TESTNAME})
ENDMACRO(MakeTest)

add_subdirectory(mctal)
add_subdirectory(meshtal)
add_subdirectory(ptrac_asc)
add_subdirectory(ptrac_bin)
add_subdirectory(ptrack_hdf5)
