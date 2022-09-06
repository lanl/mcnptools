cmake_minimum_required(VERSION 3.12.1)

include("${CMAKE_CURRENT_LIST_DIR}/Sanitizers/configuration.cmake")

get_property(shacl.cmake.sanitizers.languages GLOBAL PROPERTY ENABLED_LANGUAGES)
foreach(language IN ITEMS C CXX Fortran)
  if(language IN_LIST shacl.cmake.sanitizers.languages)
    if(NOT TARGET shacl::cmake::Sanitizers_${language})
      string(CONCAT shacl.cmake.sanitizers.generator
        "$<$<OR:$<STREQUAL:GNU,${CMAKE_${language}_COMPILER_ID}>"
              ",$<STREQUAL:Clang,${CMAKE_${language}_COMPILER_ID}>"
              ",$<STREQUAL:AppleClang,${CMAKE_${language}_COMPILER_ID}>>:"
          "shacl::cmake::detail::sanitizers>")

      add_library(shacl::cmake::Sanitizers_${language} INTERFACE IMPORTED GLOBAL)
      target_link_libraries(shacl::cmake::Sanitizers_${language} INTERFACE
        ${shacl.cmake.sanitizers.generator})

      unset(shacl.cmake.sanitizers.generator)
    endif()
  endif()
endforeach()
unset(shacl.cmake.sanitizers.languages)

include_guard(DIRECTORY)

include("${CMAKE_CURRENT_LIST_DIR}/config.cmake")
if(shacl.cmake.installation)
  get_property(
    shacl.cmake.installed_modules GLOBAL PROPERTY shacl.cmake.installed_modules)

  if(NOT "Sanitizers" IN_LIST shacl.cmake.installed_modules)
    set_property(GLOBAL APPEND PROPERTY
      shacl.cmake.installed_modules "Sanitizers")

    install(
      FILES "${CMAKE_CURRENT_LIST_FILE}"
      DESTINATION share/cmake/shacl/.cmake)

    install(
      DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/Sanitizers"
      DESTINATION share/cmake/shacl/.cmake)
  endif()

  unset(shacl.cmake.installed_modules)
endif()
