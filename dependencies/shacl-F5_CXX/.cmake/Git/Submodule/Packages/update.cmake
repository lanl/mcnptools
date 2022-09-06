include_guard(GLOBAL)

function(git_submodule_update name)
  set(source_dir "${git.submodule.packages.cache}/${name}")
  set(url "${git.submodule.package.${name}.url}")
  set(QUIET ${ARGV1})

  if(NOT DEFINED git.submodule.package.${name}.updated)
    set(git.submodule.package.${name}.updated "TRUE")
  endif()

  if(git.submodule.package.${name}.update
     AND NOT git.submodule.package.${name}.updated)

    if(NOT QUIET)
      message(STATUS "Fetching ${name} git submodule package upstream content...")
    endif()
    execute_process(
      COMMAND "${GIT_EXECUTABLE}" fetch --tags
      WORKING_DIRECTORY "${source_dir}"
      OUTPUT_QUIET
      RESULT_VARIABLE failure
      ERROR_VARIABLE error_output)

    if(failure AND NOT QUIET)
      message(WARNING "Encountered trouble while fetching from ${name} git submodule package remote repository\n")
      message("${error_output}")
    endif()

    set(branch "${git.submodule.package.${name}.branch}")

    if(NOT QUIET)
      message(STATUS "Fast forwarding ${name} git submodule package to local HEAD of tracked branch...")
    endif()
    execute_process(
      COMMAND "${GIT_EXECUTABLE}" checkout "${branch}"
      WORKING_DIRECTORY "${source_dir}"
      OUTPUT_QUIET
      RESULT_VARIABLE failure
      ERROR_VARIABLE error_output)

    if(failure)
      message("Encountered trouble checking out ${name} git reference")
      message("reference: ${branch}\n")
      message("${error_output}")
      message(FATAL_ERROR "Error while updating ${name} git submodule")
    endif()

    if(NOT QUIET)
      message(STATUS "Incorporating upstream changes to tracked branch...")
    endif()
    execute_process(
      COMMAND "${GIT_EXECUTABLE}" pull
      WORKING_DIRECTORY "${source_dir}"
      OUTPUT_QUIET
      RESULT_VARIABLE failure
      ERROR_VARIABLE error_output)

    if(failure AND NOT QUIET)
      message(WARNING "Encountered trouble while pulling from ${name} remote repository\n")
      message("${error_output}")
    endif()

    set(git.submodule.package.${name}.updated TRUE CACHE INTERNAL "" FORCE)
  endif()

  execute_process(
    COMMAND "${GIT_EXECUTABLE}" rev-parse HEAD
    WORKING_DIRECTORY "${source_dir}"
    OUTPUT_VARIABLE commit_hash
    OUTPUT_STRIP_TRAILING_WHITESPACE)

  set(git.submodule.package.${name}.hash
    "${commit_hash}"
    CACHE STRING
    "Current commit hash tracked by ${name} git submodule"
    FORCE)

  string(CONCAT content
    "set(git.submodule.package.${name}.url \"${url}\"\n"
    "  CACHE STRING \"Remote url for ${name} git submodule\"\n"
    "  FORCE)\n"
    "\n"
    "set(git.submodule.package.${name}.hash.initial \"${commit_hash}\"\n"
    "  CACHE STRING \"Current commit hash tracked by ${name} git submodule\"\n"
    "  FORCE)\n"
    "\n"
    "set(git.submodule.package.${name}.update OFF\n"
    "  CACHE BOOL \"${name} git submodule package configuration-time branch update behavior\"\n"
    "  FORCE)\n"
    "\n"
    "set(git.submodule.package.${name}.eager ON\n"
    "  CACHE STRING \"find_package will prefer to consume ${name} via submodule\"\n"
    "  FORCE)\n"
    "\n")

  set(specification_file "${git.submodule.packages.specification.output}")
  file(APPEND "${specification_file}" "${content}")
endfunction()
