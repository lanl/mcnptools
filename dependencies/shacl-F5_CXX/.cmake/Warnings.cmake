cmake_minimum_required(VERSION 3.12.1)

include(Warnings/C)
include(Warnings/CXX)
include(Warnings/Fortran)
include(Warnings/CUDA)

include_guard(DIRECTORY)

include("${CMAKE_CURRENT_LIST_DIR}/config.cmake")
if(shacl.cmake.installation)
  get_property(
    shacl.cmake.installed_modules GLOBAL PROPERTY shacl.cmake.installed_modules)

  if(NOT "Warnings" IN_LIST shacl.cmake.installed_modules)
    set_property(GLOBAL APPEND PROPERTY
      shacl.cmake.installed_modules "Warnings")

    install(FILES "${CMAKE_CURRENT_LIST_FILE}"
      DESTINATION share/cmake/shacl/.cmake)

    install(DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/Warnings"
      DESTINATION share/cmake/shacl/.cmake)
  endif()

  unset(shacl.cmake.installed_modules)
endif()

include_guard(GLOBAL)

define_property(TARGET PROPERTY WARN_ALL
BRIEF_DOCS
"Use all available warning flags subject to few or no false positives"
FULL_DOCS
"This boolean flag is used to provide a reasonably complete set of flags
 for each compiler vendor for each language")

define_property(TARGET PROPERTY WARN_ERROR
BRIEF_DOCS
"Treat warnings as errors"
FULL_DOCS
"When the boolean property is set, when supported by the compiler in use,
 compiler warnings will result in a compilation errors")

define_property(TARGET PROPERTY GNU_ENABLED_WARNINGS
BRIEF_DOCS
"GNU warning keywords flags"
FULL_DOCS
"This property contains zero or more entries specifying warning options
 used when compiling with an GNU compiler. Each entry corresponds to a
 warning keyword, e.g., specifying `strict-aliasing` will add the
 `-Wstrict-aliasing` flag.

 Multiple entries must be semicolon separated e.g. unused;unused")

define_property(TARGET PROPERTY GNU_DISABLED_WARNINGS
BRIEF_DOCS
"GNU warning keywords flags"
FULL_DOCS
"This property contains zero or more entries specifying warning options
 used when compiling with an GNU compiler. Each entry corresponds to a
 warning keyword, e.g., specifying `strict-aliasing` will add the
 `-Wno-strict-aliasing` flag.

 Multiple entries must be semicolon separated e.g. unused;unused")

define_property(TARGET PROPERTY LLVM_ENABLED_WARNINGS
BRIEF_DOCS
"LLVM warning keywords flags"
FULL_DOCS
"This property contains zero or more entries specifying warning options
 used when compiling with an LLVM compiler. Each entry corresponds to a
 warning keyword, e.g., specifying `strict-aliasing` will add the
 `-Wno-strict-aliasing` flag.

 Multiple entries must be semicolon separated e.g. unused;unused")

define_property(TARGET PROPERTY LLVM_DISABLED_WARNINGS
BRIEF_DOCS
"LLVM warning keywords flags"
FULL_DOCS
"This property contains zero or more entries specifying warning options
 used when compiling with an LLVM compiler. Each entry corresponds to a
 warning keyword, e.g., specifying `strict-aliasing` will add the
 `-Wno-strict-aliasing` flag.

 Multiple entries must be semicolon separated e.g. unused;unused")

define_property(TARGET PROPERTY MSVC_ENABLED_WARNINGS
BRIEF_DOCS
"MSVC warning keywords flags"
FULL_DOCS
"This property contains zero or more entries specifying warning options
 used when compiling with an MSVC compiler. Each entry corresponds to a
 warning keyword, e.g., specifying `4326` will add the `/we4326` flag.

 Multiple entries must be semicolon separated e.g. unused;unused")

define_property(TARGET PROPERTY MSVC_DISABLED_WARNINGS
BRIEF_DOCS
"MSVC warning keywords flags"
FULL_DOCS
"This property contains zero or more entries specifying warning options
 used when compiling with an MSVC compiler. Each entry corresponds to a
 warning keyword, e.g., specifying `4326` will add the `/wd4326` flag.

 Multiple entries must be semicolon separated e.g. unused;unused")

define_property(TARGET PROPERTY Intel_ENABLED_WARNINGS
BRIEF_DOCS
"Intel warning keywords flags"
FULL_DOCS
"This property contains zero or more entries specifying warning options
 used when compiling with an Intel compiler. Each entry corresponds to a
 warning keyword, e.g., specifying `uncalled` will add the `/warn:uncalled`
 flag or `-warn uncalled` flag as appropriate for the host platform.

 Multiple entries must be semicolon separated e.g. unused;unused")

define_property(TARGET PROPERTY Intel_DISABLED_WARNINGS
BRIEF_DOCS
"Intel warning keywords flags"
FULL_DOCS
"This property contains zero or more entries specifying warning options
 used when compiling with an Intel compiler. Each entry corresponds to a
 warning keyword, e.g., specifying `uncalled` will add the `/warn:no-uncalled`
 flag or `-warn no-uncalled` flag as appropriate for the host platform.

 Multiple entries must be semicolon separated e.g. unused;unused")
