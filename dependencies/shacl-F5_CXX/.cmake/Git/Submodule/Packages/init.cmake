include_guard(GLOBAL)

function(git_submodule_init name)
  set(source_dir "${git.submodule.packages.cache}")
  set(QUIET ${ARGV1})

  if(NOT EXISTS "${source_dir}/${name}/.git")
    set(url "${git.submodule.package.${name}.url}")

    if(NOT QUIET)
      message(STATUS "Cloning ${name} git submodule package...")
    endif()
    execute_process(
      COMMAND "${GIT_EXECUTABLE}" clone "${url}" "${source_dir}/${name}"
      OUTPUT_QUIET
      RESULT_VARIABLE failure
      ERROR_VARIABLE error_output)

    if(failure)
      message("Encountered trouble while cloning ${name} git submodule package")
      message("clone url: ${url}")
      message(FATAL_ERROR "${error_output}")
    endif()

    set(commit_hash "${git.submodule.package.${name}.hash.initial}")
    if(NOT QUIET)
      message(STATUS "Setting ${name} git submodule package to reference state...")
    endif()

    execute_process(
      COMMAND "${GIT_EXECUTABLE}" reset --hard "${commit_hash}"
      WORKING_DIRECTORY "${source_dir}/${name}"
      OUTPUT_QUIET
      RESULT_VARIABLE failure
      ERROR_VARIABLE error_output)

    if(failure)
      message("Encountered trouble while checking out commit specified for ${name} git submodule package")
      message("Commit hash: ${commit_hash}")
      message(FATAL_ERROR "${error_output}")
    endif()

    set(git.submodule.package.${name}.updated FALSE CACHE INTERNAL "" FORCE)
  endif()
endfunction()
