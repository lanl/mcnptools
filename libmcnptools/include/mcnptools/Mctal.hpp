#ifndef MCNPTOOLS_MCTAL_HPP
#define MCNPTOOLS_MCTAL_HPP 1

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include <set>

#include "mcnptools/MctalTally.hpp"
#include "mcnptools/MctalKcode.hpp"
#include "mcnptools/McnpToolsException.hpp"

namespace mcnptools {

class Mctal {
friend Mctal mergeMctals(const std::string& name, const std::vector<std::string> mctals, const unsigned int verbosity);
#ifdef __MCNPTOOLS_BOOST_MPI__
friend Mctal parallelMergeMctals(const std::string& name, const std::vector<std::string> mctals, const unsigned int verbosity);
#endif

public:
  Mctal(const std::string&);
  Mctal(const Mctal&);
  Mctal operator=(const Mctal&);

  // ~Mctal();

  std::vector<int> GetTallyList(void) const;

  std::string GetCode() const { return m_code; };
  std::string GetVersion() const { return m_version; };
  std::string GetProbid() const { return m_probid; };
  unsigned int GetDump() const { return m_dump; };
  unsigned long int GetNps() const { return m_nps; };
  unsigned long int GetRandoms() const { return m_randoms; };
  std::string GetComment() const { return m_comment; };

  MctalTally GetTally(const int&) const;
  MctalKcode GetKcode();

  std::string SummaryString() const;

  friend std::ostream& operator<<(std::ostream&, Mctal&);
  void WriteHeader(std::ostream&) const;
  std::string WriteHeader() const {
    std::stringstream ss;
    WriteHeader(ss);
    return ss.str();
  }

  Mctal IsolateTally(const int& tally, const std::string& filename) const;

private:
  Mctal();
  Mctal(const std::string&, const std::vector<int>& tallies, const std::map<int,std::streamoff>& talloc, const std::streamoff& kpos);
  std::map<int,std::streamoff> GetTallyLocations() const;
  std::streamoff GetKcodeLocation() const;

  std::string m_filename;
  mutable std::ifstream m_handle;

  std::string m_code;
  std::string m_version;
  std::string m_probid;
  unsigned int m_dump;
  unsigned long int m_nps;
  unsigned long int m_randoms;

  std::string m_comment;

  std::streamoff m_kpos;
  std::map<int,std::streamoff> m_talloc;
  std::vector<int> m_tallies;
};

}

#endif
