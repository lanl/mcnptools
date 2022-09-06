add_executable(l3dcoarsen l3dcoarsen.cpp)
set_target_properties(l3dcoarsen PROPERTIES CXX_EXTENSIONS ${mcnptools.cxx_extensions})
target_link_libraries(l3dcoarsen PRIVATE cyclops mcnptools)
set_property(TARGET l3dcoarsen APPEND PROPERTY INSTALL_RPATH ${install_rpath})

INSTALL(TARGETS l3dcoarsen DESTINATION bin)
