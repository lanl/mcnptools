if(DEFINED CMAKE_Fortran_COMPILER)
  if(NOT TARGET shacl::cmake::Warnings_Fortran)
    add_library(shacl::cmake::Warnings_Fortran INTERFACE IMPORTED GLOBAL)
    include(Warnings/Fortran/GNU)
    include(Warnings/Fortran/Intel)

    set_property(TARGET shacl::cmake::Warnings_Fortran
      APPEND PROPERTY COMPATIBLE_INTERFACE_BOOL WARN_ALL)

    set_property(TARGET shacl::cmake::Warnings_Fortran
      APPEND PROPERTY COMPATIBLE_INTERFACE_BOOL WARN_ERROR)
  endif()
endif()
