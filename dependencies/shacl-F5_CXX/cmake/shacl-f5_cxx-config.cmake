include(CMakeFindDependencyMacro)

enable_language(C)

if(TARGET shacl::F5_CXX)
  # If multiple F5_CXX's are found and imported through installations,
  # we need to check that if a newer minor version is required, that the
  # old import is at least that version
  if(shacl-F5_CXX.original.found.version)
    if( ${CMAKE_FIND_PACKAGE_NAME}_FIND_VERSION VERSION_GREATER shacl-F5_CXX.original.found.version )
      set(${CMAKE_FIND_PACKAGE_NAME}_NOT_FOUND_MESSAGE
          "shacl-F5_CXX v${${CMAKE_FIND_PACKAGE_NAME}_VERSION} was found, but previously shacl-F5_CXX v${shacl-F5_CXX.original.found.version} \
           was found and imported within this project.  Because HDF5 does not export its targets correctly and we have to manually link \
           to the HDF5 libraries, we cannot import the newer target.  Ensure the higher version is requested in all find_package calls \
           or that v${${CMAKE_FIND_PACKAGE_NAME}_VERSION} is found first (for more info see CMAKE_FIND_PACKAGE_SORT_ORDER)")
      set(${CMAKE_FIND_PACKAGE_NAME}_FOUND FALSE)
    endif()
  endif()

  # Because we cannot link to HDF5 multiple times from different project names,
  # if the target already exist, then we simply return.
  return()
endif()

if(NOT shacl-F5_CXX.original.found.version)
    set(shacl-F5_CXX.original.found.version ${shacl-F5_CXX_VERSION} CACHE INTERNAL "")
endif()

include("${CMAKE_CURRENT_LIST_DIR}/shacl-f5_cxx-targets.cmake")

# Attempt to find CMake via the two approaches possible
# Via HDF5's recommended approach.
# see USING_HDF5_CMake.txt in HDF5 release_docs for more info.
find_package(HDF5 QUIET NAMES hdf5 COMPONENTS C)

if(HDF5_FOUND AND NOT DEFINED HDF5_IS_PARALLEL)
  set(HDF5_IS_PARALLEL ${HDF5_ENABLE_PARALLEL})

  # Older versions of HDF5 are missing the "hdf5::" prefix.  This attempts linkage to both.
  if(HDF5_USE_STATIC_LIBRARIES AND TARGET hdf5::hdf5-static)
    target_link_libraries(shacl::F5_CXX INTERFACE hdf5::hdf5-static)
  elseif(HDF5_USE_STATIC_LIBRARIES AND TARGET hdf5-static)
    target_link_libraries(shacl::F5_CXX INTERFACE hdf5-static)
  elseif(NOT HDF5_USE_STATIC_LIBRARIES AND TARGET hdf5::hdf5-shared)
    target_link_libraries(shacl::F5_CXX INTERFACE hdf5::hdf5-shared)
  elseif(NOT HDF5_USE_STATIC_LIBRARIES AND TARGET hdf5-shared)
    target_link_libraries(shacl::F5_CXX INTERFACE hdf5-shared)
  else()
    message(FATAL_ERROR "CMake found HDF5 but did not find a valid target.")
  endif()
else()
  # Via FindHDF5.cmake (HDF5_ROOT / h5cc checking)
  find_dependency(HDF5 REQUIRED COMPONENTS C)

  target_link_libraries(shacl::F5_CXX INTERFACE ${HDF5_C_LIBRARIES})
  foreach(include_dir IN LISTS HDF5_INCLUDE_DIRS)
    # exclude "//include", which is erroneously included on some platforms for some spack-built HDF5 installations (Darwin P9).
    if (NOT ${include_dir} STREQUAL "//include")
      target_include_directories(shacl::F5_CXX INTERFACE $<BUILD_INTERFACE:${include_dir}>)
    endif()
  endforeach()
endif()

if(HDF5_IS_PARALLEL)
  if (NOT TARGET MPI::MPI_C)
    find_dependency(MPI COMPONENTS C)
  endif()
endif()
