backup(add_library)

macro(add_library target_)
  push(scl.DependencyInjection.target)
  set(scl.DependencyInjection.target ${target_})

  previous_add_library(${target_} "${ARGN}")

  foreach(inc IN LISTS ${scl.DependencyInjection.target}.injections)
    include(${inc})
  endforeach()

  pop(scl.DependencyInjection.target)
endmacro()
