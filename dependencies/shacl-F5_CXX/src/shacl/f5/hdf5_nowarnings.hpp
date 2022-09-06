// Suppress warnings from HDF5 and mpi headers
#ifdef __GNUC__
#pragma GCC diagnostic push

#if !defined(__clang__)
#pragma GCC diagnostic ignored "-Wsuggest-override"
#endif // not clang

#if (__GNUC__ >= 8)
#pragma GCC diagnostic ignored "-Wcast-function-type"
#endif

#endif //__GNUC__

#ifdef PHDF5
// Prevent OpenMPI from bringing in CXX dependency
#ifndef OMPI_SKIP_MPICXX
#define OMPI_SKIP_MPICXX
#endif

#include <mpi.h>

#undef OMPI_SKIP_MPICXX
#endif

#include "hdf5.h"

#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif // __GNUC__
