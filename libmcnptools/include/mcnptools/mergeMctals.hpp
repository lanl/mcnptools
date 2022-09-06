#ifndef MCNPTOOLS_MERGEMCTALS_HPP
#define MCNPTOOLS_MERGEMCTALS_HPP 1

#include "mcnptools/Mctal.hpp"

namespace mcnptools {

Mctal mergeMctals(const std::string& name, const std::vector<std::string> mctals, const unsigned int verbosity=0);

#ifdef __MCNPTOOLS_BOOST_MPI__
Mctal parallelMergeMctals(const std::string& name, const std::vector<std::string> mctals, const unsigned int verbosity=0);
#endif

} // namespace 

#endif
