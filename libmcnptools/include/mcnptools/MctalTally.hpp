#ifndef MCNPTOOLS_MCTALTALLY_HPP
#define MCNPTOOLS_MCTALTALLY_HPP 1

#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <map>
#include <vector>
#include <string>
#include <array>
#include <algorithm>

#include "mcnptools/NDArray.hpp"

#include "mcnptools/McnpToolsException.hpp"

#ifdef __MCNPTOOLS_BOOST_SERIALIZATION__
#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/map.hpp>
#endif

namespace mcnptools {

class MctalTally {
friend class Mctal;
public:

  // special tally bins
  enum MctalTallyBin {
    TFC = -1
  };

  enum MctalTFCValue {
    TFC_NPS = 0,
    TFC_VALUE = 1,
    TFC_ERROR = 2,
    TFC_FOM = 3
  };

  enum class MctalTallyBins : int {
    F = 0,
    D = 1,
    U = 2,
    S = 3,
    M = 4,
    C = 5,
    E = 6,
    T = 7
  };

  MctalTally();
//  MctalTally(const std::string, std::string artype="binary");

  int ID() const { return m_id; };
  
  int GetNextEventEstimatorType() { return m_neetype; };

  std::vector<int> GetTFCBins(int pert=0);
  int GetTFCSize(int pert=0) const {return m_tfc.at(pert).at(TFC_NPS).size();};
  double GetTFCValue(const int tfc_value, const int tfc_entry=-1, const int pert=0) const;

  double GetValue(int, int, int, int, int, int, int, int, int pert=0);
  double GetError(int, int, int, int, int, int, int, int, int pert=0);

  std::vector<double> GetValues(MctalTallyBins T, int b1, int b2, int b3, int b4, int b5, int b6, int b7, int pert=0) {
    std::vector<double> result(m_vals[pert].Shape()[static_cast<int>(T)]);
    int dummy = TFC;
    std::array<int,9> bins = {dummy, b1, b2, b3, b4, b5, b6, b7, pert};
    if (static_cast<int>(T) != 0)
      std::rotate(bins.begin(), bins.begin()+1, static_cast<int>(T)+bins.begin());
    EvalBins(bins[0],bins[1],bins[2],bins[3],bins[4],bins[5],bins[6],bins[7],bins[8]);
    bins[static_cast<int>(T)] = 0;
    for (unsigned int i = 0; i < result.size(); ++i) {
      result[i] =  m_vals[bins[8]][bins[0]][bins[1]][bins[2]][bins[3]][bins[4]][bins[5]][bins[6]][bins[7]][0];
      bins[static_cast<int>(T)]++;
    }
    return result;
  };

  std::vector<double> GetErrors(MctalTallyBins T, int b1, int b2, int b3, int b4, int b5, int b6, int b7, int pert=0) {
    std::vector<double> result(m_vals[pert].Shape()[static_cast<int>(T)]);
    int dummy = TFC;
    std::array<int, 9> bins = {dummy, b1, b2, b3, b4, b5, b6, b7, pert};
    if (static_cast<int>(T) != 0)
      std::rotate(bins.begin(), bins.begin()+1, static_cast<int>(T)+bins.begin());
    EvalBins(bins[0],bins[1],bins[2],bins[3],bins[4],bins[5],bins[6],bins[7],bins[8]);
    bins[static_cast<int>(T)] = 0;
    for (unsigned int i = 0; i < result.size(); ++i) {
      result[i] =  m_vals[bins[8]][bins[0]][bins[1]][bins[2]][bins[3]][bins[4]][bins[5]][bins[6]][bins[7]][1];
      bins[static_cast<int>(T)]++;
    }
    return result;
  };

  std::vector<double> GetFBins() const;
  std::vector<double> GetDBins() const;
  std::vector<double> GetUBins() const;
  std::vector<double> GetSBins() const;
  std::vector<double> GetMBins() const;
  std::vector<double> GetEBins() const;
  std::vector<double> GetCBins() const;
  std::vector<double> GetTBins() const;

  // statistically averge two tallies
  MctalTally& operator&=(const MctalTally&);

  std::string SummaryString();

  friend std::ostream& operator<<(std::ostream&, MctalTally&);
  void WriteToMctal(std::ostream& os) const;
  std::string WriteToMctal() const {
    std::stringstream ss;
    WriteToMctal(ss);
    return ss.str();
  }

//  void archive(const std::string, std::string artype="binary") const;

private:
  void EvalBins(int&, int&, int&, int&, int&, int&, int&, int&, int&) const;

  int m_id;
  long int m_nps;
  int m_partype;
  std::vector<int> m_partypes;
  int m_neetype;
  int m_tally_unknown;
  std::vector<std::string> m_comment;
  bool m_c2r;
  std::map<char,int> m_nactualbin;
  std::map<char,int> m_nbin;
  std::map<char,bool> m_print_bins;
  std::map<char,std::vector<double> > m_bins;
  std::map<char,bool> m_tots;
  std::map<int,cjsoft::NDArray<double,9> > m_vals;
  std::map<int,std::map<char,int> > m_tfcbins;
  std::map<int,std::map<int,std::vector<double> > > m_tfc;

#ifdef __MCNPTOOLS_BOOST_SERIALIZATION__
  friend class boost::serialization::access;

  template<class Archive>
  void serialize(Archive& ar, const unsigned int version) {
    using namespace boost::serialization;
    ar & make_nvp("id", m_id);
    ar & make_nvp("nps", m_nps);
    ar & make_nvp("partype", m_partype);
    ar & make_nvp("partypes", m_partypes);
    ar & make_nvp("neetype", m_neetype);
    ar & make_nvp("tally_unknown", m_tally_unknown);
    ar & make_nvp("comment", m_comment);
    ar & make_nvp("c2r", m_c2r);
    ar & make_nvp("nactualbin", m_nactualbin);
    ar & make_nvp("nbin", m_nbin);
    ar & make_nvp("print_bins", m_print_bins);
    ar & make_nvp("bins", m_bins);
    ar & make_nvp("tots", m_tots);
    ar & make_nvp("vals", m_vals);
    ar & make_nvp("tfcbins", m_tfcbins);
    ar & make_nvp("tfc", m_tfc);
  };
#endif
};

MctalTally operator&(const MctalTally&, const MctalTally&);
MctalTally mergeMctalTallies(const MctalTally&, const MctalTally&);

}

#endif
