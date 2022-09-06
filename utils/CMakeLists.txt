add_library(cyclops INTERFACE)
add_library(mcnptools::cyclops ALIAS cyclops)
target_include_directories(cyclops INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/cyclops-0.1.5/include>
)

add_subdirectory(l3d2vtk)
add_subdirectory(l3dcoarsen)
add_subdirectory(l3dinfo)
add_subdirectory(l3dscale)
add_subdirectory(mergemctals)
add_subdirectory(mergemeshtals)
add_subdirectory(meshtal2vtk)

find_package(TIFF QUIET)
if( TIFF_FOUND )
  add_subdirectory(mctal2rad)
else()
  message(WARNING "libtiff not found...not building mctal2rad")
endif()
