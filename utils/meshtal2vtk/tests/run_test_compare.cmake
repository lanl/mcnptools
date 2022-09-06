if ( NOT DEFINED meshtal2vtk_bin )
  message(FATAL_ERROR "Variable meshtal2vtk_bin not defined")
endif ( )

if ( NOT DEFINED meshtal2vtk_input )
  message(FATAL_ERROR "Variable meshtal2vtk_input not defined")
endif ( )

if ( NOT DEFINED testfiledir )
  message(FATAL_ERROR "Variable testfiledir not defined")
endif ( )

if ( NOT DEFINED baselinedir )
  message(FATAL_ERROR "Variable baselinedir not defined")
endif ( )

if ( NOT EXISTS "${meshtal2vtk_bin}" )
  message(FATAL_ERROR "Executable ${meshtal2vtk_bin} does not exist.")
endif ( )

execute_process( COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_CURRENT_BINARY_DIR}/${testfiledir}" )
configure_file( "${CMAKE_CURRENT_LIST_DIR}/${meshtal2vtk_input}" "${CMAKE_CURRENT_BINARY_DIR}/${meshtal2vtk_input}" COPYONLY)

execute_process(
  COMMAND ${meshtal2vtk_bin} "${CMAKE_CURRENT_BINARY_DIR}/${meshtal2vtk_input}"
  OUTPUT_VARIABLE meshtal2vtk_RUN_STDOUT
  ERROR_VARIABLE meshtal2vtk_RUN_STDERR
  RESULT_VARIABLE meshtal2vtk_RUN_RESULT
)

if ( meshtal2vtk_RUN_RESULT )
  message(STATUS "Run standard output: ${meshtal2vtk_RUN_STDOUT}.")
  message(STATUS "Run standard error: ${meshtal2vtk_RUN_STDERR}.")
  message(FATAL_ERROR "Execution of ${meshtal2vtk_bin} failed.")
endif ( )

file(GLOB OUTFILES ${testfiledir}/*.vts)
list(SORT OUTFILES)

foreach(OUTFILE ${OUTFILES})
  # Convert line endings to be consistent
  configure_file("${OUTFILE}" "${OUTFILE}.unix" NEWLINE_STYLE LF)

  get_filename_component(outfile_baseline "${OUTFILE}" NAME)
  execute_process(
    COMMAND ${CMAKE_COMMAND} -E compare_files "${OUTFILE}.unix" "${baselinedir}/${outfile_baseline}"
    RESULT_VARIABLE out_failed
  )
  if ( out_failed )
    message(SEND_ERROR "output .vts files do not match")
  endif ( out_failed )
endforeach()


