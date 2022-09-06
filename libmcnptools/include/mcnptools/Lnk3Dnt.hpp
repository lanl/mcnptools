#ifndef _MCNPTOOLS_LNK3DNT_HPP_
#define _MCNPTOOLS_LNK3DNT_HPP_

#include <vector>
#include <set>
#include <string>
#include <sstream>
#include <fstream>

#include "mcnptools/NDArray.hpp"

namespace mcnptools {

class Lnk3Dnt {
public:
  Lnk3Dnt(const int& geom, const int& nmxsp, const int& idim, const int jdim=0, const int kdim=0);
  Lnk3Dnt(std::string filename);
  Lnk3Dnt(const Lnk3Dnt& l3d);
  Lnk3Dnt operator=(const Lnk3Dnt& l3d);

  enum Lnk3DntGeom {
    SLAB1D=1,
    CYL1D=2,
    SPH1D=3,
    XY=6,
    RZ=7,
    RT=11,
    XYZ=14,
    RZT=15
  };

  // functions to query the meta data
  std::string GetName() const;
  std::string GetUser() const;
  std::string GetVersion() const;
  int GetDblparam() const;
  int GetGeom() const;
  int GetNZone() const;
  int GetICoarse() const;
  int GetJCoarse() const;
  int GetKCoarse() const;
  int GetIFine() const;
  int GetJFine() const;
  int GetKFine() const;
  int GetNMXSP() const;

  // functions to set the meta data
  Lnk3Dnt& SetName(const std::string& name);
  Lnk3Dnt& SetUser(const std::string& user);
  Lnk3Dnt& SetVersion(const std::string& version);

  // functions to query the data
  std::vector<int> GetMaterials(const size_t i, const size_t j=0, const size_t k=0) const;
  std::vector<double> GetDensities(const size_t i, const size_t j=0, const size_t k=0) const;
  const cjsoft::NDArray<int,4>& GetMaterials() const;
  const cjsoft::NDArray<double,4>& GetDensities() const;

  std::vector<int> GetMaterialIds() const;

  // functions to set the data
  Lnk3Dnt& SetMaterials(const std::vector<int>& materials, const size_t i, const size_t j=0, const size_t k=0);
  Lnk3Dnt& SetDensities(const std::vector<double>& densities, const size_t i, const size_t j=0, const size_t k=0);

  // functions to manipulate the geometry
  void Scale(const double&);
  void Coarsen (unsigned int ifact=1, unsigned int jfact=1, unsigned int kfact=1, unsigned int nmxsp=0, const bool void_0_1=true);

  // functions to query the geometry
  std::vector<double> GetIMesh() const;
  std::vector<double> GetJMesh() const;
  std::vector<double> GetKMesh() const;

  // functions to set the geometry
  Lnk3Dnt& SetIMesh(const std::vector<double>& imesh);
  Lnk3Dnt& SetJMesh(const std::vector<double>& jmesh);
  Lnk3Dnt& SetKMesh(const std::vector<double>& kmesh);

  // functions for reading the lnk3dnt
  void ReadMaterials();

  // save the lnk3dnt
  void Save(const std::string& savename) const;
  void Load(std::string filename);

private:
  // the lnk3dnt file handle
  std::string m_filename;
  std::ifstream m_handle;
  std::streampos m_matpos;
  std::streampos m_denpos;

  // struct of header data
  struct header {
    char cbuf[24];
    int dblparam;
    int geom;
    int nzone;
    int reserved00_01[2];
    int icoarse;
    int jcoarse;
    int kcoarse;
    int ifine;
    int jfine;
    int kfine;
    int reserved02_14[13];
    int nmxsp;
    int reserved15;
    int intrec;
    int ilevel;
  };

  // members for header information
  std::string m_name;
  std::string m_user;
  std::string m_version;
  int m_dblparam;
  Lnk3DntGeom m_geom;
  int m_nzone;
  int m_icoarse;
  int m_jcoarse;
  int m_kcoarse;
  int m_ifine;
  int m_jfine;
  int m_kfine;
  int m_reserved[16]; // 2 reserved + 13 reserved + 1 reserved
  int m_nmxsp;
  int m_intrec;
  int m_ilevel;

  // members for the geometry information
  std::vector<double> m_imesh;
  std::vector<int> m_iints;
  std::vector<double> m_jmesh;
  std::vector<int> m_jints;
  std::vector<double> m_kmesh;
  std::vector<int> m_kints;

  // members for the material information
  cjsoft::NDArray<int,4>    m_materials;
  cjsoft::NDArray<double,4> m_densities;
  std::set<int> m_material_ids;

  // function for computing volume
  double CalcVolume(double ii=0.0, double io=0.0, double ji=0.0, double jo=0.0, double ki=0.0, double ko=0.0);

  // function for reading the lnk3dnt header and geometry
  void ReadHeader();
  void ReadGeometry();
};

std::string l3dinfo(const std::vector<std::string>& files, const bool full=false);

} // end namespace mcnptools
#endif
