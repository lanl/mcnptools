file(GLOB testfiles "${CMAKE_CURRENT_LIST_DIR}/test*/*l3d")
set(baselinedir "${CMAKE_CURRENT_LIST_DIR}/baseline" )

foreach(testfilefullname ${testfiles})
  get_filename_component(testfilefulldir ${testfilefullname} DIRECTORY)
  get_filename_component(testfilesubdir  ${testfilefulldir} NAME)
  get_filename_component(testfilename    ${testfilefullname} NAME)

  add_test(NAME l3d2vtk_${testfilesubdir} COMMAND
    ${CMAKE_COMMAND}
      -Dl3d2vtk_bin=$<TARGET_FILE:l3d2vtk>
      -Dbaselinedir=${baselinedir}
      -Dtestfilefullname=${testfilefullname}
      -Dtestfilesubdir=${testfilesubdir}
      -Dtestfilename=${testfilename}
      -P "${CMAKE_CURRENT_SOURCE_DIR}/run_test_compare.cmake"
  )

endforeach()

