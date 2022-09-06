macro(previous_find_package)
  push(arguments_modified)
  set(arguments_modified FALSE)

  if(NOT previous_find_package_fn)
    set(previous_find_package_fn find_package)
  endif()

  push(previous_find_package_fn)

  if(NOT find_package_args STREQUAL "${ARGN}")
    set(previous_find_package_fn find_package)
    push(find_package_args)
    set(find_package_args "${ARGN}")
    set(arguments_modified TRUE)
    set(previous_find_package_fn find_package)
  else()
    set(previous_find_package_fn _${previous_find_package_fn})
  endif()

  call(${previous_find_package_fn} ${ARGN})

  if(arguments_modified)
    pop(find_package_args)
  endif()

  pop(arguments_modified)
  pop(previous_find_package_fn)
endmacro()
