backup(target_sources)

function(target_sources target tag linkage)
  if(NOT ${tag} STREQUAL "GENERATED")
    previous_target_sources(${ARGV})
    return()
  endif()

  foreach(entry IN LISTS ARGN)
    if(entry STREQUAL "PUBLIC"
        OR entry STREQUAL "PRIVATE"
        OR entry STREQUAL "INTERFACE")
      set(linkage ${entry})
    else()
      string(REGEX MATCH ".*[$]<.*>.*" generator_expression ${entry})
      if("${generator_expression}")
        message(FATAL_ERROR
          "Generator expressions are unavailable in GENERATED target_sources invocations")
      endif()

      previous_target_sources(${target} ${linkage} "${entry}")

      get_filename_component(directory ${entry} DIRECTORY)
      get_filename_component(file ${entry} NAME)
      string(REPLACE "." "_" file "${file}")

      file(RELATIVE_PATH relative_path "${PROJECT_BINARY_DIR}" "${directory}")
      string(SHA256 path_hash "${relative_path}")

      foreach(index RANGE 7 63)
        string(SUBSTRING ${path_hash} 0 ${index} potential_hash)
        set(custom_target ${target}.${potential_hash}.${file})
        if(NOT TARGET ${custom_target})
          break()
        endif()
      endforeach()

      add_custom_target(${custom_target} DEPENDS ${entry})
      set_target_properties(${custom_target} PROPERTIES FOLDER generated)
      add_dependencies(${target} ${custom_target})
      set_property(GLOBAL APPEND PROPERTY
        shacl.cmake.GeneratedSources.list "${entry}")
    endif()
  endforeach()
endfunction()
