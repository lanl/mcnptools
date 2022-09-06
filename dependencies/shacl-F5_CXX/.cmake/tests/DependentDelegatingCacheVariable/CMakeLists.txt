include(DependentDelegatingCacheVariable)

set(condition ON)
set(ddcv_foo "1")

dependent_delegating_cache_variable(ddcv_bar
  DEFAULT ddcv_foo
  TYPE STRING
  DOCSTRING "dependent delegation test case"
  CONDITION condition
  FALLBACK "2")

dependent_delegating_cache_variable(ddcv_baz
  DEFAULT ddcv_foo
  TYPE STRING
  DOCSTRING "dependent delegation test case"
  CONDITION "NOT condition"
  FALLBACK "2")

set(ddcv_fob "5" CACHE STRING "" FORCE)
dependent_delegating_cache_variable(ddcv_fob
  DEFAULT ddcv_foo
  TYPE STRING
  DOCSTRING "dependent delegation test case"
  CONDITION condition
  FALLBACK "2")

set(ddcv_fib "6" CACHE STRING "" FORCE)
dependent_delegating_cache_variable(ddcv_fib
  DEFAULT ddcv_foo
  TYPE STRING
  DOCSTRING "dependent delegation test case"
  CONDITION "NOT condition"
  FALLBACK "8")

file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/test.cpp
"
#include <cassert>
#include <string>

 int main(){
   assert(${ddcv_bar} == 1);
   assert(${ddcv_baz} == 2);
   assert(${ddcv_fob} == 5);
   assert(${ddcv_fib} == 8);
 }
")

add_executable(DependentDelegatingCacheVariable.test
  "${CMAKE_CURRENT_BINARY_DIR}/test.cpp")

add_test(
  NAME shacl.cmake.DependentDelegatingCacheVariable.test
  COMMAND DependentDelegatingCacheVariable.test)
