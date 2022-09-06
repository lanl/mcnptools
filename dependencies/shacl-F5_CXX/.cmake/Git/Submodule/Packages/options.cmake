include_guard(DIRECTORY)

include(CMakeDependentOption)
include(CMakeDependentCacheVar)

#
# This option is intended to allow an end user to opt-out of the use of git
# submodule packages by toggling this option.
#
# Here we use 'end user' to mean the person invoking CMake, rather than a
# project including this module.
#
option(git.submodule.packages "Enable git submodule package support" ON)

#
# This package supports the use of a file to reproduce previous
# configurations. We refer to these as specification files.
#
# Options in this cmake module are written in such a way that they
# will not override values specified at the command line or through
# a specification file.
#
CMAKE_DEPENDENT_CACHE_VAR(git.submodule.packages.specification
  FILEPATH
  "Path to consumed git submodule packages specification file"
  ""
  "git.submodule.packages" "")

if(git.submodule.packages.specification)
  if(NOT EXISTS git.submodule.packages.specification)
    message("git.submodule.packages.specification variable defined")
    message("specification file path: ${git.submodule.packages.specification}")
    message(FATAL_ERROR "No file exists at this path")
  endif()

  include("${git.submodule.packages.specification}")
endif()

CMAKE_DEPENDENT_OPTION(git.submodule.packages.eager
  "find_package will prefer to consume dependencies via submodules when available"
  ON "git.submodule.packages" OFF)

CMAKE_DEPENDENT_OPTION(git.submodule.packages.update
  "Update to head of branch-tracking submodules on first configuration"
  ON "git.submodule.packages" OFF)

mark_as_advanced(git.submodule.packages.update)

CMAKE_DEPENDENT_CACHE_VAR(git.submodule.packages.cache
  PATH
  "Location for git submodule packages directory clones"
  "${CMAKE_BINARY_DIR}/git-submodule-packages"
  "git.submodule.packages" "")

mark_as_advanced(git.submodule.packages.cache)

CMAKE_DEPENDENT_CACHE_VAR(git.submodule.packages.specification.output
  FILEPATH
  "Location for generated git submodule packages specification file"
  "${CMAKE_BINARY_DIR}/git-submodule-packages/specification.cmake"
  "git.submodule.packages" "")

mark_as_advanced(git.submodule.packages.specification.output)
