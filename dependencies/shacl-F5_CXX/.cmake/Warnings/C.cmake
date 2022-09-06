if(DEFINED CMAKE_C_COMPILER)
  if(NOT TARGET shacl::cmake::Warnings_C)
    add_library(shacl::cmake::Warnings_C INTERFACE IMPORTED GLOBAL)
    include(Warnings/C/GNU)
    include(Warnings/C/LLVM)
    include(Warnings/C/MSVC)
    include(Warnings/C/Intel)

    set_property(TARGET shacl::cmake::Warnings_C
      APPEND PROPERTY COMPATIBLE_INTERFACE_BOOL WARN_ALL)

    set_property(TARGET shacl::cmake::Warnings_C
      APPEND PROPERTY COMPATIBLE_INTERFACE_BOOL WARN_ERROR)
  endif()
endif()
