## the SHACL function extension implementation

In the CMake language, defining a function with the name as an existing
function is permitted. The new definition takes precedence when calling to the
function name, but the previous definition is accessible by way of a modified
name.

```
<function name>  # new definition
_<function name> # previous definition
```

By way of this mechanism, the previous functions definition is available to the
new definition allowing for behavior extension without duplication, i.e.,
they allow for an (albeit unusual) implementation of the decorator pattern.

Consider the following example.

Given a definition of a function `foo`

```
function(foo)
  message("this message comes from the original definition")
endfunction()
```

a function call `foo()` results in

```
this message comes from the original definition
```

We can redefine the `foo` function in such a way that it refers to
the original definition.

```
function(foo)
  message("this message comes from the new definition")
  _foo()
endfunction()
```

Thereafter, we can call `foo()`, just as before, but in this instance the output
has changed

```
this message comes from the new definition
this message comes from the original definition
```

Here we've augmented or 'decorated' the function.
