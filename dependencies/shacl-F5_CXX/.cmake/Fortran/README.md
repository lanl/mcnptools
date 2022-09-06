shacl::cmake::Fortran
=============

This directory provides a number of utilities for working with Fortran-based
projects in CMake.

+ [`shacl::cmake::Fortran::BackslashEscape`](./BackslashEscape/README.md)
This module provides a target and related target property which allows
a client to explicitly opt-in or opt-out of the common vendor extension to
support the backslash escape character in Fortran strings in a compiler
independent manner.

+ [`shacl::cmake::Fortran::Backtrace`](./Backtrace/README.md)
This module provides a target which allows a client to, in a
compiler independent manner, compile Fortran executables such an execution
backtrace is provided when execution fails.

+ [`shacl::cmake::Fortran::Integer`](./Integer/README.md)
This module provides a target and related target property which allows
a client to specify the size of default `integer` variables in Fortran code
in a compiler independent manner.

+ [`shacl::cmake::Fortran::ModuleDirectory`](./ModuleDirectry/README.md)
This module provides a function which associates a Fortran target with a
directory for storing compiled Fortran module files.

+ [`shacl::cmake::Fortran::Real`](./Real/README.md)n
This module provides a target and related target property which allows
a client to specify the size of default `real` variables in Fortran code
in a compiler independent manner.

+ [`shacl::cmake::Fortran::StackArraySizeLimit`](./StackArraySizeLimit/README.md)
This module provides a target and related target property which allows
a client to establish a maximum size for stack allocated automatic arrays
in a compiler independent manner. Arrays larger than this threshold fallback
to a vendor-defined memory strategy.

+ [`shacl::cmake::Fortran::Standard`](./Standard/README.md)
This module provides a target and related target property which allows a client
to specify a Fortran Standard in a compiler independent manner (for vendors
which support multiple standards)

+ [`shacl::cmake::Fortran::StandardCompliance`](./StandardCompliance/README.md)
This module provides a target which suppresses vendor extensions in Fortran
code. This target it useful when writing Fortran which must be portable between
compiler vendors.
