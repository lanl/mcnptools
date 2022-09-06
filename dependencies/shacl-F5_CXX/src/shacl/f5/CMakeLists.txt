target_sources(F5_CXX
INTERFACE 
  "${prefix}/shacl/f5/hdf5_nowarnings.hpp"
  "${prefix}/shacl/f5/H5Attribute.hpp"
  "${prefix}/shacl/f5/H5Dataset.hpp"
  "${prefix}/shacl/f5/H5Dataspace.hpp"
  "${prefix}/shacl/f5/H5File.hpp"
  "${prefix}/shacl/f5/H5Group.hpp"
  "${prefix}/shacl/f5/H5Holders.hpp"
  "${prefix}/shacl/f5/H5Properties.hpp"
  "${prefix}/shacl/f5/H5Type.hpp"
  "${prefix}/shacl/f5/H5Utilities.hpp"
  "${prefix}/shacl/f5/StringSupport.hpp"
)

add_subdirectory(H5Dataset)
add_subdirectory(H5Type)

if( shacl.F5_CXX.tests )
  add_subdirectory(test)
endif()
