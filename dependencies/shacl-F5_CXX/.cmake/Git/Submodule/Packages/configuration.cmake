include_guard(GLOBAL)

if(git.submodule.packages)
  #
  # We search for and require git iff git submodule packages are enabled.
  #
  find_package(Git REQUIRED)

  get_filename_component(
    git.submodule.packages.specification_dir
    "${git.submodule.packages.specification.output}"
    DIRECTORY)

  file(MAKE_DIRECTORY
    "${git.submodule.packages.cache}"
    "${git.submodule.packages.specification_dir}")

  string(CONCAT git.submodule.packages.specification.content
    "option(git.submodule.packages\n"
    "  \"Enable git submodule support for CMake find_package\" ON)\n"
    "\n")

  file(WRITE "${git.submodule.packages.specification.output}"
    "${git.submodule.packages.specification.content}")
endif()

#
# We eavesdrop on a few project-related variables.
#
# When any variables are written to, we cache that value in a corresonding
# (name-mangled) variable in the parent-scope. If that parent-scope is the
# calling context of `find_package` or `git_submodule_package`, these
# variables are leveraged for version checking.
#
macro(git_submodule_packages_2ParentScope var access value)
  if("${access}" STREQUAL "MODIFIED_ACCESS")
    set(git.submodule.package.${var} "${value}" PARENT_SCOPE)
  endif()
endmacro()

variable_watch(PROJECT_NAME git_submodule_packages_2ParentScope)
variable_watch(PROJECT_VERSION git_submodule_packages_2ParentScope)
variable_watch(PROJECT_VERSION_MAJOR git_submodule_packages_2ParentScope)
variable_watch(PROJECT_VERSION_MINOR git_submodule_packages_2ParentScope)
variable_watch(PROJECT_VERSION_PATCH git_submodule_packages_2ParentScope)
variable_watch(PROJECT_VERSION_TWEAK git_submodule_packages_2ParentScope)

#
# We also eavesdrop on a few variables written to when invoking
# `write_basic_package_version_file`.
#
# These variables are written to a, respective, project-specific global
# property. These are leveraged for version checking.
#
function(git_submodule_packages_2GlobalProperty var access value)
  if("${access}" STREQUAL "MODIFIED_ACCESS")
    set_property(GLOBAL PROPERTY git.submodule.package.${PROJECT_NAME}.${var}
      "${value}")
  endif()
endfunction()

variable_watch(CVF_COMPATIBILITY git_submodule_packages_2GlobalProperty)
variable_watch(CVF_VERSION git_submodule_packages_2GlobalProperty)

include(FunctionExtension)
include(DependentDelegatingOption)
