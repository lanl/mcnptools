add_executable(mergemeshtals mergemeshtals.cpp)
set_target_properties(mergemeshtals PROPERTIES CXX_EXTENSIONS ${mcnptools.cxx_extensions})
target_link_libraries(mergemeshtals PRIVATE cyclops mcnptools)
set_property(TARGET mergemeshtals APPEND PROPERTY INSTALL_RPATH ${install_rpath})

INSTALL(TARGETS mergemeshtals DESTINATION bin)
