include(Warnings)

set(C_GNU -Werror -Wall -Wextra -Wpedantic)
if(CMAKE_C_COMPILER_VERSION VERSION_GREATER "7.0")
  list(APPEND C_GNU -Wduplicated-branches)
endif()
if(CMAKE_C_COMPILER_VERSION VERSION_GREATER "6.0")
  list(APPEND C_GNU -Wduplicated-cond)
endif()
list(APPEND C_GNU -Wstrict-overflow -Wnonnull)
list(APPEND C_GNU -Wno-null-dereference -Wno-implicit)

string(REPLACE ";" "\;" C_GNU "${C_GNU}")
set(C_Clang -Werror -Wall -Wextra -Wpedantic -Wassign-enum
            -Wbad-function-cast -Wkeyword-macro
            -Wnonportable-system-include-path
            -Wsometimes-uninitialized
            -Wabstract-vbase-init
            -Warray-bounds-pointer-arithmetic
            -Wno-asm-operand-widths
            -Wno-assign-enum)
string(REPLACE ";" "\;" C_Clang "${C_Clang}")

set(C_AppleClang "${C_Clang}")

if(WIN32)
  set(C_Intel "/WX" "/W3" "/Qdiag-enable:117,230" "/Qdiag-disable:450")
else()
  set(C_Intel "-Werror" "-w2" "-diag-enable=117,230" "-diag-disable=450")
endif()

set(C_MSVC "/WX" "/W4" "/w14806" "/w14807" "/wd14810" "/wd14811")

add_executable(warnings.c.test "")
set_target_properties(warnings.c.test PROPERTIES
  WARN_ALL ON
  WARN_ERROR ON
  GNU_ENABLED_WARNINGS "strict-overflow;nonnull"
  GNU_DISABLED_WARNINGS "null-dereference;implicit"
  LLVM_ENABLED_WARNINGS "abstract-vbase-init;array-bounds-pointer-arithmetic"
  LLVM_DISABLED_WARNINGS "asm-operand-widths;assign-enum"
  Intel_ENABLED_WARNINGS "117;230"
  Intel_DISABLED_WARNINGS "450"
  MSVC_ENABLED_WARNINGS "4806;4807"
  MSVC_DISABLED_WARNINGS "4810;4811")
target_link_libraries(warnings.c.test PUBLIC shacl::cmake::Warnings_C)

file(GENERATE
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/warnings.c
  INPUT ${CMAKE_CURRENT_LIST_DIR}/warnings.c.in)

target_sources(warnings.c.test PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/warnings.c)

add_test(NAME shacl.cmake.warnings.c.test COMMAND warnings.c.test)

set_tests_properties(shacl.cmake.warnings.c.test PROPERTIES
  PASS_REGULAR_EXPRESSION "^${C_${CMAKE_C_COMPILER_ID}}\n$")

set(CXX_GNU
  -Werror -Wall -Wextra -Wpedantic -Wsuggest-override)
if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER "7.0")
  list(APPEND CXX_GNU -Wduplicated-branches)
endif()
if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER "6.0")
  list(APPEND CXX_GNU -Wduplicated-cond)
endif()
list(APPEND CXX_GNU -Wstrict-overflow -Wnonnull)
list(APPEND CXX_GNU -Wno-null-dereference)

string(REPLACE ";" "\;" CXX_GNU "${CXX_GNU}")
set(CXX_Clang
  -Werror
  -Wall
  -Wextra
  -Wpedantic
  -Wassign-enum
  -Wbad-function-cast
  -Wkeyword-macro
  -Wnonportable-system-include-path
  -Wsometimes-uninitialized
  -Wnon-virtual-dtor
  -Wrange-loop-analysis
  -Wabstract-vbase-init
  -Warray-bounds-pointer-arithmetic
  -Wno-asm-operand-widths
  -Wno-assign-enum)
string(REPLACE ";" "\;" CXX_Clang "${CXX_Clang}")

set(CXX_AppleClang "${CXX_Clang}")

if(WIN32)
  set(CXX_Intel "/WX" "/W3" "/Qdiag-enable:117,230" "/Qdiag-disable:450")
else()
  set(CXX_Intel "-Werror" "-w3" "-diag-enable=117,230" "-diag-disable=450")
endif()

set(CXX_MSVC "/WX" "/W4" "/w14806" "/w14807" "/wd14810" "/wd14811")

add_executable(warnings.cxx.test "")
set_target_properties(warnings.cxx.test PROPERTIES
  WARN_ALL ON
  WARN_ERROR ON
  GNU_ENABLED_WARNINGS "strict-overflow;nonnull"
  GNU_DISABLED_WARNINGS "null-dereference"
  LLVM_ENABLED_WARNINGS "abstract-vbase-init;array-bounds-pointer-arithmetic"
  LLVM_DISABLED_WARNINGS "asm-operand-widths;assign-enum"
  Intel_ENABLED_WARNINGS "117;230"
  Intel_DISABLED_WARNINGS "450"
  MSVC_ENABLED_WARNINGS "4806;4807"
  MSVC_DISABLED_WARNINGS "4810;4811")
target_link_libraries(warnings.cxx.test PUBLIC shacl::cmake::Warnings_CXX)

file(GENERATE
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/warnings.cpp
  INPUT ${CMAKE_CURRENT_LIST_DIR}/warnings.cpp.in)

target_sources(warnings.cxx.test PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/warnings.cpp)

add_test(NAME shacl.cmake.warnings.cxx.test COMMAND warnings.cxx.test)

set_tests_properties(shacl.cmake.warnings.cxx.test PROPERTIES
  PASS_REGULAR_EXPRESSION "^${CXX_${CMAKE_CXX_COMPILER_ID}}\n$")

if (CMAKE_CUDA_COMPILER)

  set(CUDA_NVIDIA_GNU ${CXX_GNU})
  set(CUDA_NVIDIA_Clang ${CXX_Clang})

  add_executable(warnings.cu.test "")
  set_target_properties(warnings.cu.test PROPERTIES
    WARN_ALL ON
    WARN_ERROR ON
    GNU_ENABLED_WARNINGS "strict-overflow;nonnull"
    GNU_DISABLED_WARNINGS "null-dereference"
    LLVM_ENABLED_WARNINGS "abstract-vbase-init;array-bounds-pointer-arithmetic"
    LLVM_DISABLED_WARNINGS "asm-operand-widths;assign-enum"
    Intel_ENABLED_WARNINGS "117;230"
    Intel_DISABLED_WARNINGS "450"
    MSVC_ENABLED_WARNINGS "4806;4807"
    MSVC_DISABLED_WARNINGS "4810;4811")

  target_link_libraries(warnings.cu.test PUBLIC shacl::cmake::Warnings_CUDA)

  file(GENERATE
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/warnings.cu
    INPUT ${CMAKE_CURRENT_LIST_DIR}/warnings.cu.in)

  target_sources(warnings.cu.test PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/warnings.cu)

  add_test(NAME shacl.cmake.warnings.cu.test COMMAND warnings.cu.test)

  if(CMAKE_CUDA_COMPILER_ID STREQUAL "NVIDIA")
    if (NOT DEFINED CUDA_HOST_COMPILER)
      set(CUDA_HOST_COMPILER ${CMAKE_CXX_COMPILER_ID})
    endif()
  else()
    if (NOT DEFINED CUDA_HOST_COMPILER)
      set(CUDA_HOST_COMPILER "NA")
    endif()
  endif()
  
  set_tests_properties(shacl.cmake.warnings.cu.test PROPERTIES
    PASS_REGULAR_EXPRESSION "^${CUDA_${CMAKE_CUDA_COMPILER_ID}_${CUDA_HOST_COMPILER}}\n$")

endif()

set(Fortran_GNU
"-Werror
-Wall
-Wextra
-Wpedantic
-Wcharacter-truncation
-Wrealloc-lhs"
)

if( ${CMAKE_Fortran_COMPILER_VERSION} VERSION_LESS 7.3 )
  set(Fortran_GNU
"${Fortran_GNU}
-Wno-surprising")
endif()

set(Fortran_GNU
"${Fortran_GNU}
-Wuse-without-only
-Wconversion
-Wline-truncation
-Wno-array-temporaries
-Wno-c-binding-type")

if(WIN32)
  set(Fortran_Intel
"/warn:error,stderror,all
/Qdiag-enable:8689,8685
/Qdiag-disable:8694,8695")
else()
  set(Fortran_Intel
"-warn
error,stderror,all
-diag-enable=8689,8685
-diag-disable=8694,8695")
endif()

add_executable(warnings.fortran.test "")
set_target_properties(warnings.fortran.test PROPERTIES
  WARN_ALL ON
  WARN_ERROR ON
  GNU_ENABLED_WARNINGS "conversion;line-truncation"
  GNU_DISABLED_WARNINGS "array-temporaries;c-binding-type"
  Intel_ENABLED_WARNINGS "8689;8685"
  Intel_DISABLED_WARNINGS "8694;8695")
target_link_libraries(warnings.fortran.test PUBLIC shacl::cmake::Warnings_Fortran)

file(GENERATE
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/warnings.f90
  INPUT ${CMAKE_CURRENT_LIST_DIR}/warnings.f90.in)

target_sources(warnings.fortran.test PUBLIC
  ${CMAKE_CURRENT_BINARY_DIR}/warnings.f90)

add_test(NAME shacl.cmake.warnings.fortran.test COMMAND warnings.fortran.test)
set_tests_properties(shacl.cmake.warnings.fortran.test PROPERTIES
  PASS_REGULAR_EXPRESSION "^${Fortran_${CMAKE_Fortran_COMPILER_ID}}\n$")
