configure_file(
  setup.py.in
  setup.py_configure
  @ONLY
)

find_package(Python COMPONENTS Development Interpreter REQUIRED)

Python_add_library(_mcnptools_wrap mcnptools_wrap.cpp)

target_link_libraries(_mcnptools_wrap PRIVATE mcnptools)
target_link_libraries(_mcnptools_wrap PRIVATE 
  "$<$<PLATFORM_ID:Darwin>:-undefined dynamic_lookup>"
  "$<$<OR:$<PLATFORM_ID:Windows>,$<PLATFORM_ID:CYGWIN>>:${Python_LIBRARIES}>"
)

set_target_properties(_mcnptools_wrap PROPERTIES 
  CXX_EXTENSIONS ${mcnptools.cxx_extensions}
  LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/mcnptools
)

file(COPY mcnptools DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY mcnptools_tests DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_custom_target(pybuild
  COMMAND ${Python_EXECUTABLE} setup$<CONFIG>.py build_ext
)

add_custom_target(pytest
  COMMAND ${Python_EXECUTABLE} setup$<CONFIG>.py test
  DEPENDS _mcnptools_wrap
)

file(GENERATE 
     OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/setup$<CONFIG>.py
     INPUT ${CMAKE_CURRENT_BINARY_DIR}/setup.py_configure
)

if( WIN32 )
  add_custom_command(TARGET _mcnptools_wrap
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:_mcnptools_wrap> ${CMAKE_CURRENT_BINARY_DIR}/mcnptools
  )
endif()

set(PY_MAJ_MIN "${Python_VERSION_MAJOR}.${Python_VERSION_MINOR}")

if(mcnptools.python_install STREQUAL Prefix)
  set(Python_LOCATION_COMMAND "--prefix ${CMAKE_INSTALL_PREFIX}")
elseif(mcnptools.python_install STREQUAL Global)
  set(Python_LOCATION_COMMAND "")
elseif(mcnptools.python_install STREQUAL User)
  set(Python_LOCATION_COMMAND "--user")
endif()

INSTALL(CODE "
  if(NOT \"setup$<CONFIG>.py\" STREQUAL \"setup.py\")
    execute_process(
      COMMAND ${CMAKE_COMMAND} -E copy_if_different
        \"${CMAKE_CURRENT_BINARY_DIR}/setup$<CONFIG>.py\"
        \"${CMAKE_CURRENT_BINARY_DIR}/setup.py\"
    )
  endif()

  execute_process(
    COMMAND ${Python_EXECUTABLE} -m pip install ${Python_LOCATION_COMMAND} ${CMAKE_CURRENT_BINARY_DIR}
  )
  "
)