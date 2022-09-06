string(CONCAT shacl.cmake.LinkOptions.StackSize.generator
  "$<$<OR:$<AND:$<STREQUAL:Fortran,$<TARGET_PROPERTY:LINKER_LANGUAGE>>,"
               "$<STREQUAL:Flang,${CMAKE_Fortran_COMPILER_ID}>"
         ">,"
         "$<AND:$<STREQUAL:C,$<TARGET_PROPERTY:LINKER_LANGUAGE>>,"
               "$<OR:$<C_COMPILER_ID:Clang>,"
                    "$<C_COMPILER_ID:AppleClang>"
                ">"
         ">,"
         "$<AND:$<STREQUAL:CXX,$<TARGET_PROPERTY:LINKER_LANGUAGE>>,"
               "$<OR:$<CXX_COMPILER_ID:Clang>,"
                    "$<CXX_COMPILER_ID:AppleClang>"
               ">"
         ">"
    ">:"
    "$<$<AND:$<BOOL:$<TARGET_PROPERTY:LINK_STACK_SIZE>>,"
            "$<OR:$<PLATFORM_ID:Darwin>,"
                 "$<PLATFORM_ID:CYGWIN>"
            ">"
      ">:-Wl$<COMMA>"
        "$<$<PLATFORM_ID:Darwin>:-stack_size$<COMMA>>"
        "$<$<PLATFORM_ID:CYGWIN>:--stack$<COMMA>>"
        "$<TARGET_PROPERTY:LINK_STACK_SIZE>"
    ">"
  ">")

target_link_libraries(shacl::cmake::LinkOptions::StackSize INTERFACE
  ${shacl.cmake.LinkOptions.StackSize.generator})

unset(shacl.cmake.LinkOptions.StackSize.generator)
