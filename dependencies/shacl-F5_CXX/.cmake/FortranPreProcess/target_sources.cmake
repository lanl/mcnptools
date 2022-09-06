backup(target_sources)

function(target_sources target tag linkage)
  if(NOT ${tag} STREQUAL "PREPROCESS")
    previous_target_sources(${ARGV})
    return()
  endif()

  foreach(arg ${ARGN})
    if(arg STREQUAL "PUBLIC"
        OR arg STREQUAL "PRIVATE"
        OR arg STREQUAL "INTERFACE")
      set(linkage ${arg})
    else()
      if(NOT "${arg}" MATCHES ".*\.F([0-9][0-9])?$")
        previous_target_sources(${target} PREPROCESS ${linkage} "${arg}")
        return()
      endif()

      file(RELATIVE_PATH path ${CMAKE_CURRENT_LIST_DIR} "${arg}")
      get_filename_component(directory "${path}" DIRECTORY)
      file(MAKE_DIRECTORY "${LIST_BINARY_DIR}/${directory}")

      FortranPreProcess_filename("${path}" preprocessed_path)

      FortranPreProcess(
        "${CMAKE_CURRENT_LIST_DIR}/${path}"
        "${LIST_BINARY_DIR}/${preprocessed_path}"
        DEFINE $<TARGET_PROPERTY:${target},COMPILE_DEFINITIONS>
        INCLUDE $<TARGET_PROPERTY:${target},INCLUDE_DIRECTORIES>)

      previous_target_sources(${target} GENERATED ${linkage}
        "${LIST_BINARY_DIR}/${preprocessed_path}")
    endif()
  endforeach()
endfunction()
