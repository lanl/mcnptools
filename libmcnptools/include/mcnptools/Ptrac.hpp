#ifndef _MCNPTOOLS_PTRAC_HPP_
#define _MCNPTOOLS_PTRAC_HPP_

#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <memory>

#include "mcnptools/McnpToolsException.hpp"

#include "mcnptools/ptrack/legacy/PtracNps.hpp"
#include "mcnptools/ptrack/legacy/PtracEvent.hpp"
#include "mcnptools/ptrack/legacy/PtracHistory.hpp"

#include "mcnptools/ptrack/PtracEnums.hpp"
#include "mcnptools/ptrack.hpp"

namespace mcnptools {

class Ptrac: public PtracEnums 
{
public:
  Ptrac(const std::string& filename, const unsigned int format=Ptrac::BIN_PTRAC);
  std::vector<PtracHistory> ReadHistories(const unsigned int& num);

private:

#ifdef SWIG // Work around because swig strugles with decltype
  using HDF5Parser = mcnptools::HDF5PtrackParser<mcnptools::DatasetBuffer<
    f5::Group, mcnptools::EventRecord>, mcnptools::MultiEventParser<
    std::unordered_map<mcnptools::PtrackEventType, std::function<mcnptools::PtracEvent(long unsigned int)>, std::hash<mcnptools::PtrackEventType>, std::equal_to<mcnptools::PtrackEventType>, std::allocator<std::pair<const mcnptools::PtrackEventType, std::function<mcnptools::PtracEvent(long unsigned int)> > > > > >; 
#else
  using HDF5Parser = decltype( MakeHDF5PtrackParser( std::declval<f5::File>(), std::declval<std::string>() ) );
#endif

  // Read a value from the PTRAC file
  template<class T>
  void ReadValue(T& val, bool eof_ok=false) {
    if( m_format == Ptrac::BIN_PTRAC) {
      m_handle.read( (char*) &val, sizeof(T) );
    }
    else {
      m_handle >> val;
    }

    // check for EOF
    if( !eof_ok && m_handle.eof() ) throw McnpToolsException("unexpected EOF on PTRAC file " + m_filename);
  };

  std::string m_filename;
  std::ifstream m_handle;
  PtracFormat m_format;

  int m_version;

  std::string m_code;
  std::string m_codever;
  std::string m_loddat;
  std::string m_idtm;

  std::string m_comment;

  static const std::vector<std::string> m_lines;
  std::map<std::string, int64_t> m_nument;
  std::map<std::string, std::vector<int> >  m_datent;

  //Parser for HDF5 Ptrac files.  We make it a pointer type because
  //legacy files will not be able to instantiate this parser
  std::unique_ptr<HDF5Parser> m_hdf5_parser;

  void ReadHeader();
  PtracHistory ReadHistory();
  std::vector<PtracHistory> ReadHistoriesLegacy(const unsigned int& num);

};

} // end namespace mcnptools

#endif
