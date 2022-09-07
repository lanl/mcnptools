# MCNPTools

MCNPTools is a C++ software library that is bound to Python via the Simplified
Wrapper and Interface Generator (SWIG) library.  Thus, it can be incorporated
into C++ and/or Python applications, and bindings to other languages can also be
created using a common approach.  In addition, MCNPTools includes several
utility programs to perform common tasks when interacting with [the MCNP®
code](http://mcnp.lanl.gov).

More information on how to use the library and the included utilities is
available in the `doc` directory, which represents an up-to-date form of Los
Alamos National Laboratory report LA-UR-22-28935 (see also How to Cite below).

# Building, Testing, and Installing Quick Start

The build process requires CMake, a C++11 compiler, and Python available in the
path.

To build, create a new build directory, open a command-line interface in that
directory, and run the following commands (where [path to MCNPTools source
directory] is the directory that contains this README file):

```
cmake -D CMAKE_INSTALL_PREFIX=[path to install] -D mcnptools.python_install=User [path to MCNPTools source directory]
cmake --build . --config Release
ctest --build-config Release
cmake --install . --config Release
```

To configure the build using CMake, to build the code and Python bindings, and
then to run the associated tests.  It is expected that all tests pass.  If so,
and if one wants to install to their system, then the final install command can
be used.

# Feedback & How to Contribute

Feedback, comments, requests, and bug reports can be provided through the [Github
Issue System](https://github.com/lanl/mcnptools/issues).

Contributions to fix bugs, enhance features, add new utilities, and improve
documentation can all be made through the [GitHub Pull Request
interface](https://github.com/lanl/mcnptools/pulls).

# How to Cite

If MCNPTools has been used, then a citation of its use is appreciated where the
preferred citation for MCNPTools is:

> C. R. Bates, S. R. Bolding, C. J. Josey, J. A. Kulesza, C. J. Solomon Jr., and
> A. J. Zukaitis, "The MCNPTools Package: Installation and Use", Los Alamos
> National Laboratory Tech. Report.  LA-UR-22-28935, Los Alamos, NM, USA, August
> 2022, [doi:10.2172/1884737](http://dx.doi.org/10.2172/1884737).

which has BibTeX entry:

```
@TechReport{MCNPTools,
  author      = {Cameron R. Bates and Simon R. Bolding and Colin J. Josey and Joel A. Kulesza and Solomon, Jr., Clell J. and Anthony J. Zukaitis},
  title       = {{The MCNPTools Package: Installation and Use}},
  institution = {Los Alamos National Laboratory},
  year        = {2022},
  address     = {Los Alamos, NM, USA},
  month       = aug,
  number      = {LA-UR-22-28935},
  doi         = {10.2172/1884737},
  url         = {https://www.osti.gov/biblio/1884737}
}

```

# Release

This software has been approved for open source release and has been assigned
identifier **C19081**.

# Copyright

Copyright 2022.  Triad National Security, LLC.  All rights reserved.

This program was produced under U.S. Government contract 89233218CNA000001 for
Los Alamos National Laboratory (LANL), which is operated by Triad National
Security, LLC for the U.S. Department of Energy/National Nuclear Security
Administration.  All rights in the program are reserved by Triad National
Security, LLC, and the U.S.  Department of Energy/National Nuclear Security
Administration.  The Government is granted for itself and others acting on its
behalf a nonexclusive, paid-up, irrevocable worldwide license in this material
to reproduce, prepare derivative works, distribute copies to the public, perform
publicly and display publicly, and to permit others to do so.

# License

This program is open source under the BSD-3 License.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors
   may be used to endorse or promote products derived from this software without
   specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

# Trademark Notice

MCNP® and Monte Carlo N-Particle® are registered trademarks owned by Los Alamos
National Security, LLC, manager and operator of Los Alamos National Laboratory.
Any third party use of such registered marks should be properly attributed to
Triad National Security, LLC, including the use of the ® designation as
appropriate. Any questions regarding licensing, proper use, and/or proper
attribution of Triad National Security, LLC marks should be directed to
[trademarks@lanl.gov](mailto:trademarks@lanl.gov).
