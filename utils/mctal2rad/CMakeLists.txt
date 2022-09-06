add_executable(mctal2rad mctal2rad.cpp)
set_target_properties(mctal2rad PROPERTIES CXX_EXTENSIONS ${mcnptools.cxx_extensions})
target_link_libraries(mctal2rad PRIVATE cyclops mcnptools TIFF::TIFF)
set_property(TARGET mctal2rad APPEND PROPERTY INSTALL_RPATH ${install_rpath})

INSTALL(TARGETS mctal2rad DESTINATION bin)
