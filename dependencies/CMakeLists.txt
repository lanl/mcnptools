add_subdirectory(Catch2)

set(shacl.F5_CXX.position_independent_code "${mcnptools.PIC}"
  CACHE BOOL "must set PIC before shacl-F5 is included" FORCE)
add_subdirectory(shacl-F5_CXX)
