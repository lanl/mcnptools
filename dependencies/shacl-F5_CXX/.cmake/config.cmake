include_guard(DIRECTORY)

if(NOT CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  set(subproject TRUE)
else()
  set(subproject FALSE)
endif()

if(subproject)
  set(INSTALL_SUBPROJECTS ON CACHE BOOL "Install subproject dependencies")
endif()

set(shacl.cmake.installation "default" CACHE STRING
"Install to included shacl::cmake modules used by this project.
When set to 'default', shacl::cmake projects will be installed for
the highest level project and refer to the INSTALL_SUBPROJECTS cache
variable in subprojects")

set_property(CACHE shacl.cmake.installation PROPERTY STRINGS default ON OFF)
mark_as_advanced(shacl.cmake.installation)

if(shacl.cmake.installation STREQUAL "default")
  if(subproject)
    set(shacl.cmake.install "${INSTALL_SUBPROJECTS}")
  else()
    set(shacl.cmake.install ON)
  endif()
endif()

include_guard(GLOBAL)
if(shacl.cmake.installation)
  install(FILES "${CMAKE_CURRENT_LIST_FILE}"
    DESTINATION share/cmake/shacl/.cmake)
endif()
