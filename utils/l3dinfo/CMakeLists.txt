add_executable(l3dinfo l3dinfo.cpp)
set_target_properties(l3dinfo PROPERTIES CXX_EXTENSIONS ${mcnptools.cxx_extensions})
target_link_libraries(l3dinfo PRIVATE cyclops mcnptools)
set_property(TARGET l3dinfo APPEND PROPERTY INSTALL_RPATH ${install_rpath})

INSTALL(TARGETS l3dinfo DESTINATION bin)
