## the SHACL Generated Sources module

The intent of this CMake module is improve support for the use of source files
generated during the build step in library and executable targets. While CMake
provides out-of-the-box support for this task, the mechanics are obscure and
difficult to use correctly.

In this module, the CMake `target_sources` intrinsics are extended to simplify
this process. The implementation of this functionality relies on an undocumented
behavior of CMake. Please see `documentation/function-extension.md` for more
information.

In addition to the usual signature, this module provides an additional
signature.

```
 target_sources(
   <target name> GENERATED
   <INTERFACE|PUBLIC|PRIVATE> [items1...]
   [<INTERFACE|PUBLIC|PRIVATE> [items2...] ...])
```

Note the addition of the `GENERATED` keyword with respect to the traditional
`target_sources`. Adding generated sources using this signature will establish a
[custom target](https://cmake.org/cmake/help/latest/command/add_custom_target.html)
to ensure any
[custom command](https://cmake.org/cmake/help/latest/command/add_custom_command.html)
required to generate argument files (`item1...` and `item2...`) is executed,
even if the argument target was declared in another directory. Moreover,
generated files passed `target_sources` in this fashion are marked as generated
in all parent directories.

Unfortunately, due to limitations in the `add_custom_target` function (as of the
time of writing) used in the implementation (and unlike the traditional
`target_sources`), generator expressions are not supported.

These sources are only added to their respective target in the build interface.
If these sources need be available in the install interface, a second
(traditional) call to target sources is necessary, e.g.,

```
target_sources(myTarget
  GENERATED PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/example.hpp)

target_sources(myTarget
  PUBLIC $<INSTALL_INTERFACE:include/myTarget/example.hpp>)
```
