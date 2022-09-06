#ifndef MCNPTOOLS_MCTALKCODE_HPP
#define MCNPTOOLS_MCTALKCODE_HPP 1

#include <fstream>
#include <string>
#include <sstream>
#include <map>

#include "mcnptools/NDArray.hpp"
#include "mcnptools/McnpToolsException.hpp"

#ifdef __MCNPTOOLS_BOOST_SERIALIZATION__
#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>
#endif

namespace mcnptools {

class MctalKcode {
friend class Mctal;
public:

  // kcode data types
  enum MctalKcodeValue {
    COLLSION_KEFF = 0,
    ABSORPTION_KEFF = 1,
    TRACKLENGTH_KEFF = 2,
    COLLISION_PRLT = 3,
    ABSORPTION_PRLT = 4 ,
    AVG_COLLSION_KEFF = 5 ,
    AVG_COLLSION_KEFF_STD = 6,
    AVG_ABSORPTION_KEFF = 7,
    AVG_ABSORPTION_KEFF_STD = 8,
    AVG_TRACKLENGTH_KEFF = 9,
    AVG_TRACKLENGTH_KEFF_STD = 10,
    AVG_COMBINED_KEFF = 11,
    AVG_COMBINED_KEFF_STD = 12,
    AVG_COMBINED_KEFF_BCS = 13,
    AVG_COMBINED_KEFF_BCS_STD = 14,
    COMBINED_PRLT = 15 ,
    COMBINED_PRLT_STD = 16,
    CYCLE_NPS = 17,
    AVG_COMBINED_FOM = 18
  };

  MctalKcode();
//  MctalKcode(const std::string, std::string artype="binary");

  // return a kcode value
  double GetValue(int value=-1, int cycle=-1);

  // accessors for kcode meta data
  int GetCycles() {return m_cycles;};
  int GetSettle() {return m_settle;};
  int GetNdat() {return m_ndat;};

  // statistically average two kcode data sets
  MctalKcode& operator&=(const MctalKcode&);

  std::string SummaryString();

  friend std::ostream& operator<<(std::ostream&, MctalKcode&);
  void WriteToMctal(std::ostream& os) const;
//  void archive(const std::string, std::string artype="binary") const;

private:
  long int m_nps;
  unsigned int m_cycles;
  unsigned int m_settle;
  unsigned int m_ndat;

  cjsoft::NDArray<double,2> m_kdat;

#ifdef __MCNPTOOLS_BOOST_SERIALIZATION__
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive& ar, const unsigned int version) {
    using namespace boost::serialization;
    ar & make_nvp("nps", m_nps);
    ar & make_nvp("cycles", m_cycles);
    ar & make_nvp("settle", m_settle);
    ar & make_nvp("ndat", m_ndat);
    ar & make_nvp("kdat", m_kdat);
  };
#endif
};

MctalKcode operator&(const MctalKcode&, const MctalKcode&);
MctalKcode mergeMctalKcodes(const MctalKcode&, const MctalKcode&);

} // end namespace mcnptools

#endif
