add_executable(mergemctals mergemctals.cpp)
set_target_properties(mergemctals PROPERTIES CXX_EXTENSIONS ${mcnptools.cxx_extensions})
target_link_libraries(mergemctals PRIVATE cyclops mcnptools)
set_property(TARGET mergemctals APPEND PROPERTY INSTALL_RPATH ${install_rpath})

INSTALL(TARGETS mergemctals DESTINATION bin)
