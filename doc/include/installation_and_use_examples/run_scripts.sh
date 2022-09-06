#!/bin/bash

# Simple script to compile and execute these examples.

INCPATH=/tmp/mcnptools_testing/include
LIBPATH=/tmp/mcnptools_testing/lib
LIBNAME=mcnptools

for f in `ls *cpp`; do
  g++ -Wall -I$INCPATH -L$LIBPATH -lhdf5 -l$LIBNAME $LIBPATH/libmcnptools_boost.a -o $f.exe $f
  ./$f.exe
done

for f in `ls *py`; do
  python $f
done

