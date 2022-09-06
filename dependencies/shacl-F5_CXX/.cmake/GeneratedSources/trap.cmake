function(shacl_cmake_GeneratedSources_trap _ access _ file)
  if(access STREQUAL "MODIFIED_ACCESS" AND file MATCHES ".*CMakeLists[.]txt")
    get_property(shacl.cmake.GeneratedSources.list
      GLOBAL PROPERTY shacl.cmake.GeneratedSources.list)

    foreach(shacl.cmake.GeneratedSources.entry
        IN LISTS shacl.cmake.GeneratedSources.list)
      set_source_files_properties("${shacl.cmake.GeneratedSources.entry}"
        PROPERTIES GENERATED ON)
    endforeach()
  endif()
endfunction()

variable_watch(CMAKE_PARENT_LIST_FILE shacl_cmake_GeneratedSources_trap)
