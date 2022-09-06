backup(add_executable)

macro(add_executable target_)
  push(scl.DependencyInjection.target)
  set(scl.DependencyInjection.target ${target_})

  previous_add_executable(${target_} "${ARGN}")

  foreach(inc IN LISTS ${scl.DependencyInjection.target}.injections)
    include(${inc})
  endforeach()

  pop(scl.DependencyInjection.target)
endmacro()
