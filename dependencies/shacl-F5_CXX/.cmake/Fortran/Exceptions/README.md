## shacl::cmake::Fortran::Exceptions

Fortran has no notion of language level exceptions. As such, on some platforms
(notably macOS), a Fortran compiler may choose to suppress the generation of the
linker synthesized stack unwind information commonly used to implement exception
handling in other languages. For a project composed entirely of Fortran
components, this is not an unreasonable optimization to make.

However, for multi-language projects, where one or more component langauges rely
on exception handling (such as C++ or Objective C), this behavior effectively
renders exceptions un-catchable.

This module provides an interface target which, when linked against an
executable or shared library targets, ensures the necessary stack unwinding
information is retained, even when linking with a Fortran compiler. See the
module test [directory](../../tests/Fortran/Exceptions/) for example usage.

