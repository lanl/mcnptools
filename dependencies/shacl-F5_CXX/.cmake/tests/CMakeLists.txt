cmake_minimum_required(VERSION 3.8.0)
project(shacl_cmake LANGUAGES C CXX Fortran VERSION 1.0.0)

include(CheckLanguage)
check_language(CUDA)
if (CMAKE_CUDA_COMPILER)
  enable_language(CUDA)
endif()

include(CTest)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/..)

include(CMakeDependentCacheVar)
include(Selection)
include(CMakeDependentSelection)

CMAKE_DEPENDENT_CACHE_VAR(test_dependent_var STRING
  "a variable used to test CMAKE_DEPEPENDENT_CACHE_VAR"
  "hello" "NOT use_fallback" "world")

selection(trafficLight "color of light" DEFAULT red OPTIONS red yellow green)
selection(activity "programmer activity" OPTIONS hotrocks sleep reddit)

CMAKE_DEPENDENT_SELECTION(test_selection
  "a variable used to test CMAKE_DEPENDENT_SELECTION"
  DEFAULT red OPTIONS red yellow green
  CONDITION "NOT selection_fallback" green)

#add_subdirectory(Backports)
add_subdirectory(Fortran)
add_subdirectory(FortranPreProcess)
add_subdirectory(Intel)
add_subdirectory(LinkOptions)
add_subdirectory(GeneratedSources)
add_subdirectory(Sanitizers)
add_subdirectory(Git/Submodule/Packages)
add_subdirectory(DependencyInjection)
add_subdirectory(Warnings)
add_subdirectory(DelegatingCacheVariable)
add_subdirectory(DelegatingOption)
add_subdirectory(DelegatingSelection)
add_subdirectory(DependentDelegatingCacheVariable)
add_subdirectory(DependentDelegatingOption)
add_subdirectory(DependentDelegatingSelection)

# KEEP THIS AT THE BOTTOM
add_subdirectory(FunctionExtension)
