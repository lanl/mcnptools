#ifndef MCNPTOOLS_MERGEMESHTALS_HPP
#define MCNPTOOLS_MERGEMESHTALS_HPP

#include "mcnptools/Meshtal.hpp"

namespace mcnptools {

Meshtal mergeMeshtals(const std::string& name, const std::vector<std::string> meshtals, const unsigned int verbosity=0);

#ifdef __MCNPTOOLS_BOOST_MPI__
Meshtal parallelMergeMeshtals(const std::string& name, const std::vector<std::string> meshtals, const unsigned int verbosity=0);
#endif

} // namespace 

#endif
