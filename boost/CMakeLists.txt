add_library(mcnptools_boost STATIC "")
add_library(mcnptools::mcnptools_boost ALIAS mcnptools_boost)
target_compile_features(mcnptools_boost PUBLIC cxx_std_11)
set_target_properties(mcnptools_boost PROPERTIES 
  CXX_EXTENSIONS ${mcnptools.cxx_extensions}
  POSITION_INDEPENDENT_CODE ${mcnptools.PIC}
)

target_compile_definitions(mcnptools_boost PUBLIC 
  $<$<PLATFORM_ID:Windows>:BOOST_ALL_NO_LIB>)

target_include_directories(mcnptools_boost PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<INSTALL_INTERFACE:include>
)

list(APPEND boost_lib_dirs regex)
foreach(_dir IN LISTS boost_lib_dirs)
  add_subdirectory(libs/${_dir})
endforeach()

install(DIRECTORY boost DESTINATION ${CMAKE_INSTALL_PREFIX}/include
        PATTERN "*.hpp"
)

install(TARGETS mcnptools_boost EXPORT mcnptools_boostTargets
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
  INCLUDES DESTINATION include
)

install(EXPORT mcnptools_boostTargets
  FILE mcnptools_boostTargets.cmake
  NAMESPACE mcnptools::
  DESTINATION lib/cmake/mcnptools_boost
)

configure_file("cmake/mcnptools_boostConfig.cmake.in" "mcnptools_boostConfig.cmake" @ONLY)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/mcnptools_boostConfig.cmake" 
  DESTINATION lib/cmake/mcnptools_boost
)


