include(DependentDelegatingSelection)

set(condition ON)
set(dds_foo "oog")

dependent_delegating_selection(dds_bar
  DEFAULT dds_foo
  OPTIONS "oog" "osh" "poy" "mop" "lor" "lop" "nom"
  DOCSTRING "dependent delegation test case"
  CONDITION condition
  FALLBACK "osh")

dependent_delegating_selection(dds_baz
  DEFAULT dds_foo
  OPTIONS "oog" "osh" "poy" "mop" "lor" "lop" "nom"
  DOCSTRING "dependent delegation test case"
  CONDITION "NOT condition"
  FALLBACK "poy")

set(dds_fob "mop" CACHE STRING "" FORCE)
dependent_delegating_selection(dds_fob
  DEFAULT dds_foo
  OPTIONS "oog" "osh" "poy" "mop" "lor" "lop" "nom"
  DOCSTRING "dependent delegation test case"
  CONDITION condition
  FALLBACK "lor")

set(dds_fib "lop" CACHE STRING "" FORCE)
dependent_delegating_selection(dds_fib
  DEFAULT dds_foo
  OPTIONS "oog" "osh" "poy" "mop" "lor" "lop" "nom"
  DOCSTRING "dependent delegation test case"
  CONDITION "NOT condition"
  FALLBACK "nom")

file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/test.cpp
"
#include <cassert>
#include <string>

 int main(){
   assert(std::string(\"${dds_bar}\") == \"oog\");
   assert(std::string(\"${dds_baz}\") == \"poy\");
   assert(std::string(\"${dds_fob}\") == \"mop\");
   assert(std::string(\"${dds_fib}\") == \"nom\");
 }
")

add_executable(DependentDelegatingSelection.test
  "${CMAKE_CURRENT_BINARY_DIR}/test.cpp")

add_test(
  NAME shacl.cmake.DependentDelegatingSelection.test
  COMMAND DependentDelegatingSelection.test)
