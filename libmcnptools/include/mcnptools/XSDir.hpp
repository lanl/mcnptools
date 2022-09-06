#ifndef MCNPTOOLS_XSDIR_HPP
#define MCNPTOOLS_XSDIR_HPP 1

#include <string>
#include <cstdlib>
#include <map>
#include <set>

namespace mcnptools {

class XSDir {
public:
  XSDir(std::string xsdir="xsdir", std::string datapath=std::getenv("DATAPATH"));

  bool Has(unsigned int zaid, std::string library="") const;

private:
  std::map<std::string, std::set<unsigned int> > m_library_zaids;
};

} // namespace mcnptools

#endif
