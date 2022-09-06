Git Submodule Packages
=====

Quickstart
-----

For the impatient, in order to incorporate this functionality into your CMake
project, add the following lines to your project's highest level
`CMakeLists.txt` before any call to `find_package`.

```cmake
list(APPEND CMAKE_MODULE_PATH <path to shacl::cmake>)
include(Git/Submodule/Packages)
```

Thereafter, any call to `find_package` will be submodule-aware.

Introduction
-----

The intent of this CMake module to provide a means of managing, recording,
and sharing software dependency usage during software development. This module
leverages the git version control system's 'submodule' feature to express
dependencies between (potentially many layers) of CMake projects.

For those unfamiliar with Git submodules (or would just appreciate a refresher),
an introduction to the feature is provided in the
[Git Submodules 101](README/git-submodules-101.md) file in the README directory.

From the perspective of the author of a project CMake description, this module
provides an extension to the intrinsic `find_package` macro, providing
integration to allow git submodules to act as a back-end.

Goals
-----

The fundemental goal of this CMake module is to support the use of

+ a modular repository structure
+ third party dependencies

where

+ system administration support is poor
+ external network access is unavailable
+ there exists no internal git hosting service

Beyond that the module aims to

1. be easily incorporated into existing projects
1. work within the existing vocabulary of CMake
1. improve software reproducibility
1. fallback gracefully to traditional `find_package` behavior
1. accomodate additional extension to the `find_package` behavior
1. avoid making life harder for the person packaging software

Controlling Behavior
-----

This module provides a number of CMake cache variables to tailor the extended
behavior of `find_package`. These cache variables are broken into two
categories: fine-grain and course-grain.

#### Coarse Grain ####

Coarse grain variables define the default and package-independent behavior.

---

+ `git.submodule.packages`
  + type: boolean
  + default: `ON`
  + condition: N/A

This option allows an end user (meaning the person invoking CMake) to opt-out
of the use Git submodule usage in `find_package`. This option is useful for
those interested in packaging software with fixed version dependencies as part
of a larger ecosystem (as is conventional with Debian's `apt`, Red Hat's `dnf`,
or Microsoft's `vcpkg` package managers).

-----

+ `git.submodule.packages.cache`
  + type: directory path
  + default: `${CMAKE_BINARY_DIR}/git-submodule-packages`
  + condition: `git.submodule.packages`

During configuration, `find_package` will lazily clone any requested submodule
packages. This variable specifies a root directory to which any such repository
should be cloned (and later found). Note that this directory is, by default, in
the binary directory. This allows an end user to experiment using multiple build
configurations leveraging different dependency versions.

This option is labeled as advanced and when `git.submodule.packages` is `OFF`,
this option does not appear in the cache.

-----

+ `git.submodule.packages.specification`
  + type: file path
  + default: N/A
  + condition: N/A

During each configuration, CMake will write a file recording the usage of Git
submodules (see the `git.submodule.packages.specification.output` variable).
These file can later be consumed to reproduce that environment. This variable
is used to specify the path to such a file, which will be used to reproduce
the described environment.

-----

+ `git.submodule.packages.specification.output`
  + type: file path
  + default: `${CMAKE_BINARY_DIR}/git-submodule-packages/specification.cmake`
  + condition: `git.submodule.packages`

During each configuration, CMake will write a file recording the usage of Git
submodules (see the `git.submodule.packages.specification.output` variable).
This includes an absolute url to each submodule remote and a corresponding Git
commit hash. These file can later be consumed to reproduce that environment.
This variable is used to specify the path to write this specification file.

This option is labeled as advanced and when `git.submodule.packages` is `OFF`,
this option does not appear in the cache.

-----

+ `git.submodule.packages.update`
  + type: boolean
  + default: `ON`
  + condition: `git.submodule.packages`

This controls an aspect of behavior when working with Git submodules with an
associated branch. When `ON`, CMake will fast-forward the cloned dependency
repositories to the HEAD of their respective associated branches.

This option is labeled as advanced and when `git.submodule.packages` is `OFF`,
this option does not appear in the cache.

-----

+ `git.submodule.packages.eager`
  + type: boolean
  + default: `ON`
  + condition: `git.submodule.packages`

When `ON`, the `find_package` command will eagerly consume dependencies via
Git submodule (when a corresponding submodule is available). When `OFF`, the
`find_package` command will first attempt to find a system installation of the
dependency, falling back to Git submodules only if a system installation could
not be found.

When `git.submodule.packages` is `OFF`, this option does not appear in the
cache.

----

#### Fine Grain ####


Fine grain variables allow users to specify behavior and inspect state on a
package-by-package basis. All fine-grain variables are marked as advanced to
avoid overwhelming newcomers.

----

+ `git.submodule.package.<package name>`
  + type: boolean
  + default: `ON`
  + condition: `git.submodule.packages`

This option allows a user to opt-out of the use of a Git submodule associated
with a particular package. When `git.submodule.packages` is `OFF`, this option
does not appear in the cache.

----

+ `git.submodule.package.<package name>.branch`
  + type: string
  + default: see description
  + condition: see description

This string appears in the cache for submodules with an associated branch. This
variable is intended to be read-only and is meant for debugging. When
`git.submodule.packages` or `git.submodule.package.<package name>` is `OFF`,
this option does not appear in the cache.

----

+ `git.submodule.package.<package name>.eager`
  + type: enumeration
  + default: `default`
  + values: `default`, `ON`, `OFF`
  + condition: `git.submodule.package.<package name>`

This is a package-specific analog to the `git.submodule.packages.eager` coarse
grain option. When set to default, this option reproduces the value specified in
the coarse grain option. Otherwise, when `ON`, the `find_package` command will
eagerly consume the package via Git submodule. When `OFF`, the `find_package`
command will first attempt to find a system installation of the dependency,
falling back to the Git submodule only if a system installation could not be
found.

When `git.submodule.packages` or `git.submodule.package.<package name>` is
`OFF`, this option does not appear in the cache.

----

+ `git.submodule.package.<package name>.update`
  + type: enumeration
  + default: `default`
  + values: `default`, `ON`, `OFF`
  + condition: see description

This is a package-specific analog to the `git.submodule.packages.update` coarse
grain option and only appears in the cache when
`git.submodule.package.<package name>.branch` is populated. When set to default,
this option reproduces the value specified in the coarse grain option.
Otherwise, when `ON`, CMake will fast-forward the cloned dependency repository
to the HEAD of its associated branch.

When `git.submodule.packages` or `git.submodule.package.<package name>` is
`OFF`, this option does not appear in the cache.

----

+ `git.submodule.package.<package name>.url`
  + type: string
  + default: see description
  + condition: `git.submodule.package.<package name>`

This string appears in the cache for each Git submodule. This variable is
intended to be read-only and is meant for debugging. When
`git.submodule.packages` or `git.submodule.package.<package name>` is `OFF`,
this option does not appear in the cache.

----

Implementation
----

The implementation of the `find_package` command depends on a CMake developer
feature that allows for the extension and/or overwriting of CMake command. The
use of this functionality for purposes beyond debugging is controversial;
[Daniel Pfeifer](https://gitlab.kitware.com/purpleKarrot) (of
[Effective CMake](https://youtu.be/bsXLMQ6WgIk)) suggests this sort of extension
as a reasonable use of the facility, while
[Craig Scott](https://gitlab.kitware.com/craig.scott) (of
[Professional CMake](https://crascit.com/professional-cmake/))
[discourages](https://crascit.com/2018/09/14/do-not-redefine-cmake-commands/)
this practice.

For those siding with Scott, this module also provides an alternative command,
`git_submodule_package` which does not leverage developer features. This
command falls back to `find_package` when the git submodule package
functionality is disabled (via fine-grain or coarse-grain cache variables), but
otherwise does not interoperate with the intrinsic command.
