cmake_minimum_required(VERSION 3.12.1)

include("Git/Submodule/Packages/options")
include("Git/Submodule/Packages/configuration")
include("Git/Submodule/Packages/list")
include("Git/Submodule/Packages/init")
include("Git/Submodule/Packages/update")
include("Git/Submodule/Packages/package")
include("Git/Submodule/Packages/find_package")
git_submodule_list()

include_guard(DIRECTORY)
include("${CMAKE_CURRENT_LIST_DIR}/../../config.cmake")

if(shacl.cmake.installation)
  get_property(
    shacl.cmake.installed_modules GLOBAL PROPERTY shacl.cmake.installed_modules)

  if(NOT "Git/Submodule/Packages" IN_LIST shacl.cmake.installed_modules)
    set_property(GLOBAL APPEND PROPERTY
      shacl.cmake.installed_modules "Git/Submodule/Packages")

    install(FILES "${CMAKE_CURRENT_LIST_FILE}"
      DESTINATION share/cmake/shacl/.cmake/Git/Submodule)

    install(DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/Packages"
      DESTINATION share/cmake/shacl/.cmake/Git/Submodule)
  endif()

  unset(shacl.cmake.installed_modules)
endif()

