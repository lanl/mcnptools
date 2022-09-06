include_guard(GLOBAL)
define_property(TARGET PROPERTY asan
  BRIEF_DOCS "Enable address sanitizer"
  FULL_DOCS "AddressSanitizer (aka ASan) is a tool that detects memory error")

define_property(TARGET PROPERTY msan
  BRIEF_DOCS "Enable memory sanitizer"
  FULL_DOCS "MemorySanitizer (aka MSan) is a tool that detects reads from uninitialized memory")

define_property(TARGET PROPERTY tsan
  BRIEF_DOCS "Enable thread sanitizer"
  FULL_DOCS "ThreadSanitizer (aka TSan) is a tool that detects data races.")

define_property(TARGET PROPERTY ubsan
  BRIEF_DOCS "Enable undefined behavior sanitizer"
  FULL_DOCS "UndefinedBehaviorSanitizer (aka UBSan) is a tool that detects undefined behavior")

option(shacl.sanitizer.asan.default
  "Default address sanitizer behavior (ON/OFF)" OFF)
mark_as_advanced(shacl.sanitizer.asan.default)

option(shacl.sanitizer.msan.default
  "Default memory sanitizer behavior (ON/OFF)" OFF)
mark_as_advanced(shacl.sanitizer.msan.default)

option(shacl.sanitizer.tsan.default
  "Default thread sanitizer behavior (ON/OFF)" OFF)
mark_as_advanced(shacl.sanitizer.tsan.default)

option(shacl.sanitizer.ubsan.default
  "Default undefined behavior sanitizer behavior (ON/OFF)" OFF)
mark_as_advanced(shacl.sanitizer.ubsan.default)

add_library(shacl::cmake::detail::sanitizers INTERFACE IMPORTED GLOBAL)

set(shacl.cmake.sanitizers.asan address)
set(shacl.cmake.sanitizers.msan memory)
set(shacl.cmake.sanitizers.tsan thread)
set(shacl.cmake.sanitizers.ubsan undefined)

foreach(sanitizer IN ITEMS asan msan tsan ubsan)
  string(CONCAT shacl.cmake.sanitizers.generator
    "${shacl.cmake.sanitizers.generator}"
    "$<$<OR:$<BOOL:$<TARGET_PROPERTY:${sanitizer}>>"
          ",$<BOOL:${shacl.sanitizer.${sanitizer}.default}>>:"
      "-fsanitize=${shacl.cmake.sanitizers.${sanitizer}};"
      "-fno-omit-frame-pointer;"
      "-fno-sanitize-recover=all;>")
endforeach()

unset(shacl.cmake.sanitizers.asan)
unset(shacl.cmake.sanitizers.msan)
unset(shacl.cmake.sanitizers.tsan)
unset(shacl.cmake.sanitizers.ubsan)

target_compile_options(shacl::cmake::detail::sanitizers INTERFACE
  ${shacl.cmake.sanitizers.generator})

target_link_libraries(shacl::cmake::detail::sanitizers INTERFACE
  ${shacl.cmake.sanitizers.generator})

unset(shacl.cmake.sanitizer.generator)

set_property(TARGET shacl::cmake::detail::sanitizers
  APPEND PROPERTY COMPATIBLE_INTERFACE_BOOL asan)

set_property(TARGET shacl::cmake::detail::sanitizers
  APPEND PROPERTY COMPATIBLE_INTERFACE_BOOL msan)

set_property(TARGET shacl::cmake::detail::sanitizers
  APPEND PROPERTY COMPATIBLE_INTERFACE_BOOL tsan)

set_property(TARGET shacl::cmake::detail::sanitizers
  APPEND PROPERTY COMPATIBLE_INTERFACE_BOOL ubsan)

