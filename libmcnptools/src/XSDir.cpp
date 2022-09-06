#include "mcnptools/XSDir.hpp"
#include "mcnptools/StringOps.hpp"

#include <fstream>
#include <sstream>

#ifndef __MCNPTOOLS_BOOST_REGEX__
  #include <regex>
  namespace reg = std;
#else
  #include <boost/regex.hpp>
  namespace reg = boost;
#endif

namespace mcnptools {

XSDir::XSDir(std::string xsdir, std::string datapath) {
  
  std::string path = xsdir;
  if( datapath != "" )
    path = datapath + "/" + xsdir;

  std::ifstream xsdir_file(path.c_str());

  std::string line;
  cjsoft::stringops::getline(xsdir_file,line);
  reg::smatch n;
  reg::regex dmatch("^directory\\s*$");
  while( ! reg::regex_search(line, n, dmatch) && ! xsdir_file.eof() ) {
    cjsoft::stringops::getline(xsdir_file,line);
  }

  // NEEDED: do something if end of file
  
  while( ! xsdir_file.eof() ) {
    cjsoft::stringops::getline(xsdir_file,line);

    reg::regex zmatch("^\\s*(\\d+)\\.(\\d{2}[a-z])");
    if( reg::regex_search(line,n,zmatch) ) {
      unsigned int zaid;
      std::string lib;

      std::stringstream(n[1]) >> zaid;
      lib = n[2];

      m_library_zaids[lib].insert(zaid);
    }
  }
}

bool XSDir::Has(unsigned int zaid, std::string library) const {
  bool retval = false;

  if( library == "" ) {
    for(std::map<std::string, std::set<unsigned int> >::const_iterator it = m_library_zaids.begin(); it != m_library_zaids.end(); it++) {
      if( it->second.find(zaid) != it->second.end() )
        retval = true;
    }
  }
  else {
    if( m_library_zaids.find(library) != m_library_zaids.end() ) {
      if( m_library_zaids.at(library).find(zaid) != m_library_zaids.at(library).end() )
        retval = true;
    }
  }

  return retval;
}

} // namepsace
