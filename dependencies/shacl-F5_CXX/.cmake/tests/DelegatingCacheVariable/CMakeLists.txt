include(DelegatingCacheVariable)

set(dcv_foo "1")
delegating_cache_variable(dcv_bar
  DEFAULT dcv_foo
  TYPE STRING
  DOCSTRING "delegation test case")

set(dcv_fob "OFF")
delegating_cache_variable(dcv_fib
  DEFAULT dcv_fob
  TYPE BOOL
  DOCSTRING "delegation test case")

set(dcv_baz "1" CACHE STRING "" FORCE)
set(dcv_bax "0")
delegating_cache_variable(dcv_baz
  DEFAULT dcv_bax
  TYPE STRING
  DOCSTRING "non-delegation test case")

set(dcv_gob "0" CACHE STRING "" FORCE)
set(dcv_gof "1")
delegating_cache_variable(dcv_gob
  DEFAULT dcv_gof
  TYPE STRING
  DOCSTRING "non-delegation test case")

file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/test.cpp
"
#include <cassert>
#include <string>

 int main(){
   assert(${dcv_bar});
   assert(std::string(\"${dcv_fib}\") == \"OFF\");
   assert(${dcv_baz});
   assert(${dcv_gob} == 0);
 }
")

add_executable(DelegatingCacheVariable.test
  "${CMAKE_CURRENT_BINARY_DIR}/test.cpp")

add_test(
  NAME shacl.cmake.DelegatingCacheVariable.test
  COMMAND DelegatingCacheVariable.test)
