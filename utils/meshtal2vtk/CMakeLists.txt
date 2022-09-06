add_executable(meshtal2vtk meshtal2vtk.cpp)
set_target_properties(meshtal2vtk PROPERTIES CXX_EXTENSIONS ${mcnptools.cxx_extensions})
target_link_libraries(meshtal2vtk PRIVATE cyclops mcnptools)
set_property(TARGET meshtal2vtk APPEND PROPERTY INSTALL_RPATH ${install_rpath})

install(TARGETS meshtal2vtk DESTINATION bin)

add_subdirectory(tests)
