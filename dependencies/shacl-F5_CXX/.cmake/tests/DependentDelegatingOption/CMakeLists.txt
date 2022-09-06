include(DependentDelegatingOption)

set(condition ON)
set(ddo_foo "1")

dependent_delegating_option(ddo_bar
  DEFAULT ddo_foo
  DOCSTRING "dependent delegation test case"
  CONDITION condition
  FALLBACK "2")

dependent_delegating_option(ddo_baz
  DEFAULT ddo_foo
  DOCSTRING "dependent delegation test case"
  CONDITION "NOT condition"
  FALLBACK "OFF")

set(ddo_fob "ON" CACHE STRING "" FORCE)
dependent_delegating_option(ddo_fob
  DEFAULT ddo_foo
  DOCSTRING "dependent delegation test case"
  CONDITION condition
  FALLBACK "OFF")

set(ddo_fib "ON" CACHE STRING "" FORCE)
dependent_delegating_option(ddo_fib
  DEFAULT ddo_foo
  DOCSTRING "dependent delegation test case"
  CONDITION "NOT condition"
  FALLBACK "OFF")

file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/test.cpp
"
#include <cassert>
#include <string>

 int main(){
   assert(std::string(\"${ddo_bar}\") == \"ON\");
   assert(std::string(\"${ddo_baz}\") == \"OFF\");
   assert(std::string(\"${ddo_fob}\") == \"ON\");
   assert(std::string(\"${ddo_fib}\") == \"OFF\");
 }
")

add_executable(DependentDelegatingOption.test
  "${CMAKE_CURRENT_BINARY_DIR}/test.cpp")

add_test(
  NAME shacl.cmake.DependentDelegatingOption.test
  COMMAND DependentDelegatingOption.test)
