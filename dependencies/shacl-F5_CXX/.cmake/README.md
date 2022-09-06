# the shacl::cmake module repository

## Introduction
This repository is intended to act as a shared collection of cmake modules for
use in shacl projects. The intent is that this repository will exist as a git
subtree in host projects.

## Adding shacl::cmake to an existing git repository

Given Git v1.7.11 or later, adding another repository as a subtree is a
straight-forward process. To start, ensure the host repository is in a clean
state, i.e., there are no uncommitted modifications to tracked content.

```
git status
```

If uncommitted modifications to tracked content exist, either record or discard
each modification as appropriate.

+ recording modifications

```
git add <modified file>; git commit
```

+ discarding modifications

```
git checkout -- <modified file>
```

Of course, on a fresh clone, no modifications will exist and this step can be
skipped.

It's recommended to add the cmake subtree to highest level source directory of
the git repository. Once you've navigated there, the following command will
create a `cmake` directory and populate it with the contents of the master
branch of this repository.

```
git subtree add --prefix cmake <git remote address>/shacl/cmake.git master [--squash]
```

The `--squash` flag here is important for those who maintain a curated commit
log. Without this flag, the entire commit history of the `shacl::cmake`
repository would be appended to the host project directory. With this flag
specified, the `shacl::cmake` repository commit history is compressed to a
single commit in the host project. In either case, there will also be a merge
commit added to the log.

## Using shacl::cmake

Once the `shacl::cmake` subtree has been added to your repository, there's the
matter of exposing it's modules to the host CMake build system generator.
Navigate to the highest level source directory of the host repository and add
the following line to the the `CMakeLists.txt`.

```
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
```

There after `shacl::cmake` modules are available by way of the CMake `include`
function, e.g.,

```
include(path/to/module)
```
