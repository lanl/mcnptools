#ifndef MCNPTOOLS_MESHTALTALLY_HPP
#define MCNPTOOLS_MESHTALTALLY_HPP 1

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <string>

#include "mcnptools/NDArray.hpp"

#include "mcnptools/McnpToolsException.hpp"

#ifdef __MCNPTOOLS_BOOST_SERIALIZATION__
#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>
#endif

namespace mcnptools {

class MeshtalTally {
friend class Meshtal;
public:

  enum MeshTallyType {
    XYZ = 0,
    RZT = 1
  };

  MeshtalTally();

  int ID() const { return m_id; };

  std::vector<double> GetXRBounds() const { return m_xr_bounds; };
  std::vector<double> GetYZBounds() const { return m_yz_bounds; };
  std::vector<double> GetZTBounds() const { return m_zt_bounds; };
  std::vector<double> GetEBounds()  const { return m_e_bounds; };
  std::vector<double> GetTBounds()  const { return m_t_bounds; };

  std::vector<double> GetXRBins() const { return m_xr; };
  std::vector<double> GetYZBins() const { return m_yz; };
  std::vector<double> GetZTBins() const { return m_zt; };
  std::vector<double> GetEBins()  const { return m_e; };
  std::vector<double> GetTBins()  const { return m_t; };

  MeshtalTally::MeshTallyType GetType() const { return MeshTallyType(m_type); };

  double GetValue(int,int,int,int e=-1,int t=-1);
  double GetError(int,int,int,int e=-1,int t=-1);
  double GetVolume(int,int,int);

  // statistically average two tallies
  MeshtalTally& operator&=(const MeshtalTally&);

  std::string SummaryString();

  friend std::ostream& operator<<(std::ostream&, MeshtalTally&);
  void WriteToMeshtal(std::ostream& os) const;
  std::string WriteToMeshtal() const {
    std::stringstream ss;
    WriteToMeshtal(ss);
    return ss.str();
  }

private:
    int m_id;
    int64_t m_nps;
    unsigned int m_type;

    std::vector<double> m_xr_bounds;
    std::vector<double> m_yz_bounds;
    std::vector<double> m_zt_bounds;
    std::vector<double> m_e_bounds;
    std::vector<double> m_t_bounds;

    std::vector<double> m_xr;
    std::vector<double> m_yz;
    std::vector<double> m_zt;
    std::vector<double> m_e;
    std::vector<double> m_t;
    bool m_etot;
    bool m_ttot;

    cjsoft::NDArray<double,6> m_data;

    cjsoft::NDArray<double,3> m_volumes;

#ifdef __MCNPTOOLS_BOOST_SERIALIZATION__
    // boost serialization stuff
    friend class boost::serialization::access;

    template <class Archive>
    void serialize(Archive& ar, const unsigned int version) {
        using namespace boost::serialization;
        ar & make_nvp("id", m_id);
        ar & make_nvp("nps", m_nps);
        ar & make_nvp("geom", m_type);

        ar & make_nvp("ibounds", m_xr_bounds);
        ar & make_nvp("jbounds", m_yz_bounds);
        ar & make_nvp("kbounds", m_zt_bounds);
        ar & make_nvp("ebounds", m_e_bounds);
        ar & make_nvp("tbounds", m_t_bounds);

        ar & make_nvp("ival", m_xr);
        ar & make_nvp("jval", m_yz);
        ar & make_nvp("kval", m_zt);
        ar & make_nvp("eval", m_e);
        ar & make_nvp("tval", m_t);

        ar & make_nvp("etot", m_etot);
        ar & make_nvp("ttot", m_ttot);

        ar & make_nvp("data", m_data);

        ar & make_nvp("volumes", m_volumes);
    }
#endif
};

MeshtalTally operator&(const MeshtalTally&, const MeshtalTally&);
MeshtalTally mergeMeshtalTallies(const MeshtalTally&, const MeshtalTally&);

} // namespace mcnptools

#endif
