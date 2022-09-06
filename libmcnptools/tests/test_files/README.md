# MCNPTools test output files

This directory contains various examples of MCNP outputs generated from
MCNP used to check that parsers correctly reads in desired results.
`inpgen` was used to generate tests for `meshtal`, `mctal`, and legacy `ptrac`
tests. The HDF5 ptrac files `testing.h5` and `parallel_testing.h5` were
generated with `hdf5_ptrack_testing.inp`, with the latter being generated
using task parallelism.  The `ptrack_all_events.h5` file was generated using
the `hdf5_ptrack_all.inp` input.

The `mctal` file is modified by hand to include 3-digit Fortran-formatted
floating-point values that lack a `e` to denote the exponential.

The `mctal_malformed` file is modified by hand to include a dash between the
date and time to create an incorrect string, which should result in an MCNPTools
exception.
