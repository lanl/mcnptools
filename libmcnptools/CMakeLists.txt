add_library(mcnptools "")
add_library(mcnptools::mcnptools ALIAS mcnptools)
target_compile_features(mcnptools PUBLIC cxx_std_11)
set_target_properties(mcnptools PROPERTIES 
  CXX_EXTENSIONS ${mcnptools.cxx_extensions}
  POSITION_INDEPENDENT_CODE ${mcnptools.PIC}
)

# configure the MCNPTools_Config.hpp file
CONFIGURE_FILE(MCNPTools_Config.hpp.in ${CMAKE_CURRENT_BINARY_DIR}/include/mcnptools/MCNPTools_Config.hpp @ONLY)

# MPI related options
target_compile_definitions(mcnptools PUBLIC 
  $<$<BOOL:${mcnptools.mpi}>:__MCNPTOOLS_BOOST_SERIALIZATION__>
  $<$<BOOL:${mcnptools.mpi}>:__MCNPTOOLS_BOOST_MPI__>
)
target_link_libraries(mcnptools PUBLIC 
  $<$<BOOL:${mcnptools.mpi}>:Boost::mpi>
  $<$<BOOL:${mcnptools.mpi}>:MPI::MPI_CXX>
)

if(MSVC_VERSION GREATER_EQUAL 1910)
  # On MSVC, must set standards-conformant mode.
  target_compile_options(mcnptools PUBLIC "/permissive-")
elseif(MSVC)
  message(FATAL_ERROR "MSVC++ 14.10 or newer (Visual Studio 2017) is required to use this library.")
endif()

# REGEX related options
target_compile_definitions(mcnptools PRIVATE 
  $<$<BOOL:${mcnptools.boost_regex}>:__MCNPTOOLS_BOOST_REGEX__>
)
target_link_libraries(mcnptools PUBLIC 
  $<$<BOOL:${mcnptools.boost_regex}>:$<IF:$<BOOL:${Boost_REGEX_FOUND}>,Boost::regex,mcnptools::mcnptools_boost>>
)

target_link_libraries(mcnptools PUBLIC shacl::F5_CXX)

target_include_directories(mcnptools PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

add_subdirectory(src)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/include/mcnptools/MCNPTools_Config.hpp"
  DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/mcnptools"
)

install(DIRECTORY include/ 
      DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
      PATTERN "*.hpp"
)

include(GNUInstallDirs)
install(TARGETS mcnptools EXPORT mcnptoolsTargets
  LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
  ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
  RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
  INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
)

install(EXPORT mcnptoolsTargets
  FILE mcnptoolsTargets.cmake
  NAMESPACE mcnptools::
  DESTINATION "${CMAKE_INSTALL_DATADIR}/cmake/mcnptools"
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file("mcnptoolsConfigVersion.cmake"
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion
)

configure_file("cmake/mcnptoolsConfig.cmake.in" "mcnptoolsConfig.cmake" @ONLY)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/mcnptoolsConfig.cmake" 
              "${CMAKE_CURRENT_BINARY_DIR}/mcnptoolsConfigVersion.cmake"
       DESTINATION "${CMAKE_INSTALL_DATADIR}/cmake/mcnptools"
)

if(mcnptools.testing)
  add_subdirectory(tests)
endif()
