add_executable(l3d2vtk l3d2vtk.cpp)
set_target_properties(l3d2vtk PROPERTIES CXX_EXTENSIONS ${mcnptools.cxx_extensions})
target_link_libraries(l3d2vtk PRIVATE cyclops mcnptools)
set_property(TARGET l3d2vtk APPEND PROPERTY INSTALL_RPATH ${install_rpath})

INSTALL(TARGETS l3d2vtk DESTINATION bin)

add_subdirectory(tests)
