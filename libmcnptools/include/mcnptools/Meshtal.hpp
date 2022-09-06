#ifndef MCNPTOOLS_MESHTAL_HPP
#define MCNPTOOLS_MESHTAL_HPP 1

#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>
#include <string>
#include <map>
#include <vector>
#include <set>

#include "mcnptools/MeshtalTally.hpp"
#include "mcnptools/McnpToolsException.hpp"

namespace mcnptools {

class Meshtal {
friend Meshtal mergeMeshtals(const std::string& name, const std::vector<std::string> meshtal_names, const unsigned int verbosity);
#ifdef __MCNPTOOLS_BOOST_MPI__
friend Meshtal parallelMergeMeshtals(const std::string& name, const std::vector<std::string> meshtal_names, const unsigned int verbosity);
#endif
public:
  Meshtal(const std::string& fname);
  Meshtal(const Meshtal&);
  Meshtal operator=(const Meshtal&);

  std::vector<int> GetTallyList(void) const;

  std::string GetCode() const { return m_code; };
  std::string GetVersion() const { return m_version; };
  std::string GetProbid() const { return m_probid; };
  std::string GetComment() const { return m_comment; };
  long int GetNps() const { return m_nps; };

  MeshtalTally GetTally(const int&) const;

  std::string SummaryString() const;

  friend std::ostream& operator<<(std::ostream&, Meshtal&);
  void WriteHeader(std::ostream&) const;
  std::string WriteHeader() const {
    std::stringstream ss;
    WriteHeader(ss);
    return ss.str();
  };


  // Meshtal IsolateTally(const int& tally, const std::string& filename) const;

private:
  Meshtal();
  Meshtal(const std::string& fname, const std::map<int,std::streamoff>& tallies);
  std::string m_filename;
  mutable std::ifstream m_handle;

  std::string m_code;
  std::string m_version;
  std::string m_probid;
  long int m_nps;

  std::string m_comment;

  std::map<int,std::streamoff> m_tallies;
};

}

#endif
