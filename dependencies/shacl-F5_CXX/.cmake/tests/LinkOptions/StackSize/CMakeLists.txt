include(LinkOptions/StackSize)

add_executable(use_stack_size_f90 ${CMAKE_CURRENT_LIST_DIR}/use_stack_size.f90)
set_target_properties(use_stack_size_f90 PROPERTIES LINK_STACK_SIZE 1024000)
target_link_libraries(use_stack_size_f90 PRIVATE shacl::cmake::LinkOptions::StackSize)

add_executable(use_stack_size_c ${CMAKE_CURRENT_LIST_DIR}/use_stack_size.c)
set_target_properties(use_stack_size_c PROPERTIES LINK_STACK_SIZE 1024000)
target_link_libraries(use_stack_size_c PRIVATE shacl::cmake::LinkOptions::StackSize)

add_executable(use_stack_size_cpp ${CMAKE_CURRENT_LIST_DIR}/use_stack_size.cpp)
set_target_properties(use_stack_size_cpp PROPERTIES LINK_STACK_SIZE 1024000)
target_link_libraries(use_stack_size_cpp PRIVATE shacl::cmake::LinkOptions::StackSize)

add_custom_target(run_use_stack_size_f90 ALL
  COMMAND use_stack_size_f90
  DEPENDS use_stack_size_f90)

add_custom_target(run_use_stack_size_c ALL
  COMMAND use_stack_size_c
  DEPENDS use_stack_size_c)

add_custom_target(run_use_stack_size_cpp ALL
  COMMAND use_stack_size_cpp
  DEPENDS use_stack_size_cpp)
