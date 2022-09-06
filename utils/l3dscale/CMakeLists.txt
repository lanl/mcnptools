add_executable(l3dscale l3dscale.cpp)
set_target_properties(l3dscale PROPERTIES CXX_EXTENSIONS ${mcnptools.cxx_extensions})
target_link_libraries(l3dscale PRIVATE cyclops mcnptools)
set_property(TARGET l3dscale APPEND PROPERTY INSTALL_RPATH ${install_rpath})

INSTALL(TARGETS l3dscale DESTINATION bin)
