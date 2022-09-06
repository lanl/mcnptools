cmake_minimum_required(VERSION 3.12.1)
include_guard(DIRECTORY)

include("${CMAKE_CURRENT_LIST_DIR}/../config.cmake")
if(shacl.cmake.installation)
  get_property(
    shacl.cmake.installed_modules GLOBAL PROPERTY shacl.cmake.installed_modules)

  if(NOT "Fortran/StackArraySizeLimit" IN_LIST shacl.cmake.installed_modules)
    set_property(GLOBAL APPEND PROPERTY
      shacl.cmake.installed_modules "Fortran/StackArraySizeLimit")

    install(
      FILES "${CMAKE_CURRENT_LIST_FILE}"
      DESTINATION share/cmake/shacl/.cmake/Fortran)

    install(
      DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/StackArraySizeLimit"
      DESTINATION share/cmake/shacl/.cmake/Fortran)
  endif()

  unset(shacl.cmake.installed_modules)
endif()

if(NOT DEFINED CMAKE_Fortran_COMPILER)
  return()
endif()

include_guard(GLOBAL)
define_property(TARGET PROPERTY Fortran_STACK_ARRAY_SIZE_LIMIT
BRIEF_DOCS
"A integer size limit (in kilobytes) above which arrays should not be stack allocated"
FULL_DOCS
"This behavior varies widely between Fortran compilers.

In the case of Intel ifort, automatic and temporary arrays which exceed this
limit are allocated on the heap. This is the ideal behavior.

On GNU gfortran (at least in version 9.0 and earlier) any stack allocated
variable which exceeds this limit are allocated in static storage. As this value
is specified in kilobytes, most primitive and user-defined data-types will be
excluded from this behavior for non-zero Fortran_STACK_ARRAY_SIZE_LIMIT values.
That said, the use of static storage for these arrays is not thread-safe in the
absense of other constructs (e.g. thread private).

In the case of the PGI compiler, this option is only available in a coarse-grain
form; either all automatic and temporary arrays are stack-allocated or none are.
As such, the value of Fortran_STACK_ARRAY_SIZE_LIMIT is irrelevant for this
compiler; this flag is dispatched based on whether the
Fortran_STACK_ARRAY_SIZE_LIMIT is defined on a target linking to the
shacl::cmake::Fortran::StackArraySizeLimit target.")

add_library(shacl::cmake::Fortran::StackArraySizeLimit INTERFACE IMPORTED GLOBAL)

include(Fortran/StackArraySizeLimit/GNU)
include(Fortran/StackArraySizeLimit/Intel)
include(Fortran/StackArraySizeLimit/PGI)
