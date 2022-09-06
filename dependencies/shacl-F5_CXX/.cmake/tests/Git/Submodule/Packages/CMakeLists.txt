include(CTest)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/../../../..)

include(Git/Submodule/Packages)

set(JSON_BuildTests OFF CACHE INTERNAL "")
find_package(json 3.0.2 REQUIRED)
git_submodule_package(Catch2)

file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/bar.cpp
"#include <cassert>

 int main(){
   assert(\"${git.submodule.package.json.hash}\" == \"7bfc406ded0434c438dd22139a8baa97f2ffa90e\" );
   assert(\"${json_FOUND}\" == \"TRUE\" );
   assert(\"${git.submodule.package.Catch2.branch}\" == \"master\" );
 }
")

add_executable(Git.Submodule.Packages.test ${CMAKE_CURRENT_BINARY_DIR}/bar.cpp)

add_test(
  NAME shacl.cmake.Git.Submodule.Packages.test
  COMMAND Git.Submodule.Packages.test)
