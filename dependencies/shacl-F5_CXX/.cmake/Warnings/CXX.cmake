if(DEFINED CMAKE_CXX_COMPILER)
  if(NOT TARGET shacl::cmake::Warnings_CXX)
    add_library(shacl::cmake::Warnings_CXX INTERFACE IMPORTED GLOBAL)
    include(Warnings/CXX/GNU)
    include(Warnings/CXX/LLVM)
    include(Warnings/CXX/MSVC)
    include(Warnings/CXX/Intel)

    set_property(TARGET shacl::cmake::Warnings_CXX
      APPEND PROPERTY COMPATIBLE_INTERFACE_BOOL WARN_ALL)

    set_property(TARGET shacl::cmake::Warnings_CXX
      APPEND PROPERTY COMPATIBLE_INTERFACE_BOOL WARN_ERROR)
  endif()
endif()

