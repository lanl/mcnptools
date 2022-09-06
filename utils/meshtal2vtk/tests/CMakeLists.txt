set(TESTFILES test001/meshtal)
list(SORT TESTFILES)

set(baselinedir "${CMAKE_CURRENT_LIST_DIR}/baseline" )

foreach(TESTFILE ${TESTFILES})
  get_filename_component(TESTFILEDIR ${TESTFILE} DIRECTORY)
  get_filename_component(testid ${TESTFILEDIR} NAME)

  add_test(NAME meshtal2vtk_${testid} COMMAND
    ${CMAKE_COMMAND}
      -Dmeshtal2vtk_bin=$<TARGET_FILE:meshtal2vtk>
      -Dmeshtal2vtk_input=${TESTFILE}
      -Dtestfiledir=${TESTFILEDIR}
      -Dbaselinedir=${baselinedir}/${testid}
      -P "${CMAKE_CURRENT_SOURCE_DIR}/run_test_compare.cmake"
  )
endforeach()

