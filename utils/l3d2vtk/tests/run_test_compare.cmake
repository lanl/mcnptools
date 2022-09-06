
set(l3d2vtk_input "${CMAKE_CURRENT_BINARY_DIR}/${testfilesubdir}/${testfilename}")
string(REGEX REPLACE ".l3d" ".vts" l3d2vtk_output "${testfilesubdir}/${testfilename}")

configure_file("${testfilefullname}" "${l3d2vtk_input}" COPYONLY)

execute_process(
  COMMAND ${l3d2vtk_bin} -V "${l3d2vtk_input}" "${l3d2vtk_output}"
  OUTPUT_VARIABLE l3d2vtk_run_stdout
  ERROR_VARIABLE  l3d2vtk_run_stderr
  RESULT_VARIABLE l3d2vtk_run_result
)

if ( l3d2vtk_RUN_RESULT )
  message(STATUS "Run standard output: ${l3d2vtk_run_stdout}.")
  message(STATUS "Run standard error: ${l3d2vtk_run_stderr}.")
  message(FATAL_ERROR "Execution of ${l3d2vtk_bin} failed.")
endif ( )

# Convert line endings to be consistent
configure_file("${l3d2vtk_output}" "${l3d2vtk_output}.unix" NEWLINE_STYLE LF)

execute_process(
  COMMAND ${CMAKE_COMMAND} -E
    compare_files
    "${l3d2vtk_output}.unix"
    "${baselinedir}/${l3d2vtk_output}"
  RESULT_VARIABLE out_failed
)

if ( out_failed )
  message(SEND_ERROR "output .vts files do not match")
endif ( out_failed )

