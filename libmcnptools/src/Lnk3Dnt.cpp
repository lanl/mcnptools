#include "mcnptools/Constants.hpp"
#include "mcnptools/Lnk3Dnt.hpp"

#include <algorithm>
#include <cmath>
#include <cassert>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <set>

namespace mcnptools {

// constructor
//   make new
Lnk3Dnt::Lnk3Dnt(const int& geom, const int& nmxsp, const int& idim, const int jdim, const int kdim) {
  m_geom = (static_cast<Lnk3DntGeom>(geom));
  m_name = "lnk3dnt ";
  m_user = "mcnp    ";
  m_version = "tools   ";
  m_dblparam = 4;
  m_nzone = 0;
  m_icoarse = idim;
  m_jcoarse = jdim > 0 ? jdim : 1;
  m_kcoarse = kdim > 0 ? kdim : 1;
  m_ifine = idim;
  m_jfine = jdim > 0 ? jdim : 1;
  m_kfine = kdim > 0 ? kdim : 1;
  m_nmxsp = nmxsp;
  m_intrec = 0;
  m_ilevel = 0;

  cjsoft::NDArray<int,4>::shaper_type mshaper;
  m_materials.Reshape( mshaper[m_ifine > 0 ? m_ifine : 1][m_jfine > 0 ? m_jfine : 1][m_kfine > 0 ? m_kfine : 1][m_nmxsp], cjsoft::NDArray<int,4>::COLMAJOR );

  cjsoft::NDArray<double,4>::shaper_type dshaper;
  m_densities.Reshape( dshaper[m_ifine > 0 ? m_ifine : 1][m_jfine > 0 ? m_jfine : 1][m_kfine > 0 ? m_kfine : 1][m_nmxsp], cjsoft::NDArray<double,4>::COLMAJOR );
}

//   load existing
Lnk3Dnt::Lnk3Dnt(std::string filename) : m_filename(filename) {
  Load(filename);
}

Lnk3Dnt::Lnk3Dnt(const Lnk3Dnt& l3d) {
  if( l3d.m_handle.is_open() ) {
    m_filename = l3d.m_filename;
    m_matpos = l3d.m_matpos;
    m_denpos = l3d.m_denpos;

    m_handle.open(m_filename.c_str(), std::fstream::in);
    if( l3d.m_material_ids.size() == 0 ) {
      m_handle.seekg(m_matpos);
    }
    else {
      m_handle.seekg(0, m_handle.end);
    }
  }

  m_name            = l3d.m_name;
  m_user            = l3d.m_user;
  m_version         = l3d.m_version;
  m_dblparam        = l3d.m_dblparam;
  m_geom            = l3d.m_geom;
  m_nzone           = l3d.m_nzone;
  m_icoarse         = l3d.m_icoarse;
  m_jcoarse         = l3d.m_jcoarse;
  m_kcoarse         = l3d.m_kcoarse;
  m_ifine           = l3d.m_ifine;
  m_jfine           = l3d.m_jfine;
  m_kfine           = l3d.m_kfine;
  for(size_t i=0; i<16; i++) {
    m_reserved[i]        = l3d.m_reserved[i];
  }
  m_nmxsp           = l3d.m_nmxsp;
  m_intrec          = l3d.m_intrec;
  m_ilevel          = l3d.m_ilevel;

  m_imesh           = l3d.m_imesh;
  m_iints           = l3d.m_iints;
  m_jmesh           = l3d.m_jmesh;
  m_jints           = l3d.m_jints;
  m_kmesh           = l3d.m_kmesh;
  m_kints           = l3d.m_kints;

  // members for the material information
  m_materials       = l3d.m_materials;
  m_densities       = l3d.m_densities;
  m_material_ids    = l3d.m_material_ids;
};

Lnk3Dnt Lnk3Dnt::operator=(const Lnk3Dnt& l3d) {
  Lnk3Dnt retval(l3d);
  return retval;
};

// functions to query the meta data
std::string Lnk3Dnt::GetName() const {
  return m_name;
}

std::string Lnk3Dnt::GetUser() const {
  return m_user;
}

std::string Lnk3Dnt::GetVersion() const {
  return m_version;
}

int Lnk3Dnt::GetDblparam() const {
  return m_dblparam;
}

int Lnk3Dnt::GetGeom() const {
  return (int) m_geom;
}

int Lnk3Dnt::GetNZone() const {
  return m_nzone;
}

int Lnk3Dnt::GetICoarse() const {
  return m_icoarse;
}

int Lnk3Dnt::GetJCoarse() const {
  return m_jcoarse;
}

int Lnk3Dnt::GetKCoarse() const {
  return m_kcoarse;
}

int Lnk3Dnt::GetIFine() const {
  return m_ifine;
}

int Lnk3Dnt::GetJFine() const {
  return m_jfine;
}

int Lnk3Dnt::GetKFine() const {
  return m_kfine;
}

int Lnk3Dnt::GetNMXSP() const {
  return m_nmxsp;
}

// functions to set the meta data
Lnk3Dnt& Lnk3Dnt::SetName(const std::string& name) {
  m_name = name;
  return *this;
}

Lnk3Dnt& Lnk3Dnt::SetUser(const std::string& user) {
  m_user = user;
  return *this;
}

Lnk3Dnt& Lnk3Dnt::SetVersion(const std::string& version) {
  m_version = version;
  return *this;
}

// functions to query the data
std::vector<int> Lnk3Dnt::GetMaterials(const size_t i, const size_t j, const size_t k) const {
  std::vector<int> retval;
  for(size_t m=0; m<m_nmxsp; m++) {
    if( m_materials[i][j][k][m] == 1 ||
        m_materials[i][j][k][m] == 0 ) {
      break;
    }

    retval.push_back(m_materials[i][j][k][m]);
  }

  return retval;
}

std::vector<double> Lnk3Dnt::GetDensities(const size_t i, const size_t j, const size_t k) const {
  std::vector<double> retval;
  for(size_t m=0; m<m_nmxsp; m++) {
    if( m_materials[i][j][k][m] == 1 ||
        m_materials[i][j][k][m] == 0 ) {
      break;
    }

    retval.push_back(m_densities[i][j][k][m]);
  }

  return retval;
}

const cjsoft::NDArray<int,4>& Lnk3Dnt::GetMaterials() const {
  return m_materials;
}

const cjsoft::NDArray<double,4>& Lnk3Dnt::GetDensities() const {
  return m_densities;
}

std::vector<int> Lnk3Dnt::GetMaterialIds() const {
  return std::vector<int>(m_material_ids.begin(),m_material_ids.end());
}

// functions to set the data
Lnk3Dnt& Lnk3Dnt::SetMaterials(const std::vector<int>& materials, const size_t i, const size_t j, const size_t k) {
  assert( materials.size() == m_nmxsp );
  for(size_t m=0; m<m_nmxsp; m++) {
    m_materials[i][j][k][m] = materials[m];
    m_material_ids.insert( materials[m] );
  }

  m_nzone = m_material_ids.size();

  return *this;
}

Lnk3Dnt& Lnk3Dnt::SetDensities(const std::vector<double>& densities, const size_t i, const size_t j, const size_t k) {
  assert( densities.size() == m_nmxsp );
  for(size_t m=0; m<m_nmxsp; m++) {
    m_densities[i][j][k][m] = densities[m];
  }
  return *this;
}

// get the mesh in the i direction
std::vector<double> Lnk3Dnt::GetIMesh() const {
  std::vector<double> retval;

  // insert the starting value
  retval.push_back(m_imesh.at(0));

  // iterate over all other values
  for(unsigned int i=1; i<m_imesh.size(); i++) {
    double ds = (m_imesh.at(i)-m_imesh.at(i-1)) / m_iints.at(i-1);
    for(int l=1; l<=m_iints.at(i-1); l++) {
      retval.push_back(m_imesh.at(i-1) + l*ds);
    }
  }

  return retval;
}

// get the mesh in the j direction
std::vector<double> Lnk3Dnt::GetJMesh() const {
  std::vector<double> retval;

  // check that mesh is at least 2D
  if( m_geom >= XY ) {
    // insert the starting value
    retval.push_back(m_jmesh.at(0));

    // iterate over all other values
    for(unsigned int j=1; j<m_jmesh.size(); j++) {
      double ds = (m_jmesh.at(j)-m_jmesh.at(j-1)) / m_jints.at(j-1);
      for(int l=1; l<=m_jints.at(j-1); l++) {
        retval.push_back(m_jmesh.at(j-1) + l*ds);
      }
    }
  }

  return retval;
}

// get the mesh in the k direction
std::vector<double> Lnk3Dnt::GetKMesh() const {
  std::vector<double> retval;

  // check that mesh is 3D
  if( m_geom >= XYZ ) {
    // insert the starting value
    retval.push_back(m_kmesh.at(0));

    // iterate over all other values
    for(unsigned int k=1; k<m_kmesh.size(); k++) {
      double ds = (m_kmesh.at(k)-m_kmesh.at(k-1)) / m_kints.at(k-1);
      for(int l=1; l<=m_kints.at(k-1); l++) {
        retval.push_back(m_kmesh.at(k-1) + l*ds);
      }
    }
  }

  return retval;
}

// set the mesh in the i direction
Lnk3Dnt& Lnk3Dnt::SetIMesh(const std::vector<double>& imesh) {
  assert( imesh.size() == m_icoarse+1 );
  m_imesh = imesh;
  m_iints = std::vector<int>(m_ifine,1);
  return *this;
}

// set the mesh in the j direction
Lnk3Dnt& Lnk3Dnt::SetJMesh(const std::vector<double>& jmesh) {
  assert( jmesh.size() == m_jcoarse+1 );
  m_jmesh = jmesh;
  m_jints = std::vector<int>(m_jfine,1);
  return *this;
}

// set the mesh in the j direction
Lnk3Dnt& Lnk3Dnt::SetKMesh(const std::vector<double>& kmesh) {
  assert( kmesh.size() == m_kcoarse+1 );
  m_kmesh = kmesh;
  m_kints = std::vector<int>(m_kfine,1);
  return *this;
}

// scale the lnk3dnt geometry values
void Lnk3Dnt::Scale(const double &s) {
  for(unsigned int i=0; i<m_imesh.size(); i++) {
    m_imesh[i] *= s;
  }

  for(unsigned int j=0; j<m_jmesh.size(); j++) {
    m_jmesh[j] *= s;
  }

  for(unsigned int k=0; k<m_kmesh.size(); k++) {
    m_kmesh[k] *= s;
  }
}

// coarsen the lnk3dnt geometry
void Lnk3Dnt::Coarsen(unsigned int ifact, unsigned int jfact, unsigned int kfact, unsigned int nmxsp, const bool void_0_1) {
  if( nmxsp == 0 )
    nmxsp = m_nmxsp;

  if( static_cast<int>(nmxsp) > m_nmxsp )
    nmxsp = m_nmxsp;

  std::vector<double> imesh = GetIMesh();
  std::vector<double> jmesh = GetJMesh();
  std::vector<double> kmesh = GetKMesh();

  unsigned int nimesh = imesh.size()-1;
  unsigned int njmesh = jmesh.size()-1;
  unsigned int nkmesh = kmesh.size()-1;

  // determine if coarsening factors are viable (if not set to unity)
  if( nimesh / ifact == 0 ) ifact = nimesh;
  if( njmesh / jfact == 0 ) jfact = njmesh;
  if( nkmesh / kfact == 0 ) kfact = nkmesh;

  // setup front and back spacings in each direction
  unsigned int nifnt, nibck, nires, nitot;
  nires = nimesh % ifact;
  nifnt = ((nimesh / ifact) / 2) + ((nimesh / ifact) % 2);
  nibck = ((nimesh / ifact) / 2);
  nitot = nifnt + nires + nibck;
  std::vector<unsigned int> nistp;
  for(unsigned int i=0; i<nifnt; i++) nistp.push_back(ifact);
  for(unsigned int i=0; i<nires; i++) nistp.push_back(1);
  for(unsigned int i=0; i<nibck; i++) nistp.push_back(ifact);

  std::vector<double> new_imesh;
  new_imesh.push_back(imesh[0]);
  unsigned int iloc=0;
  for(unsigned int i=0; i<nistp.size(); i++) {
    iloc += nistp[i];
    new_imesh.push_back( imesh.at(iloc) );
  }

  unsigned int njtot;
  std::vector<double> new_jmesh;
  std::vector<unsigned int> njstp;
  if( m_geom > CYL1D ) {
    unsigned int njfnt, njbck, njres;
    njres = njmesh % jfact;
    njfnt = ((njmesh / jfact) / 2) + ((njmesh / jfact) % 2);
    njbck = ((njmesh / jfact) / 2);
    njtot = njfnt + njres + njbck;
    for(unsigned int j=0; j<njfnt; j++) njstp.push_back(jfact);
    for(unsigned int j=0; j<njres; j++) njstp.push_back(1);
    for(unsigned int j=0; j<njbck; j++) njstp.push_back(jfact);

    new_jmesh.push_back(jmesh[0]);
    unsigned int jloc=0;
    for(unsigned int j=0; j<njstp.size(); j++) {
      jloc += njstp[j];
      new_jmesh.push_back( jmesh.at(jloc) );
    }
  }
  else {
    jmesh = {0.0,0.0};
    njtot = 1;
    new_jmesh = {0.0,0.0};
    njstp = {1};
  }

  unsigned int nktot;
  std::vector<double> new_kmesh;
  std::vector<unsigned int> nkstp;
  if( m_geom > RT ) {
    unsigned int nkfnt, nkbck, nkres;
    nkres = nkmesh % kfact;
    nkfnt = ((nkmesh / kfact) / 2) + ((nkmesh / kfact) % 2);
    nkbck = ((nkmesh / kfact) / 2);
    nktot = nkfnt + nkres + nkbck;
    for(unsigned int k=0; k<nkfnt; k++) nkstp.push_back(kfact);
    for(unsigned int k=0; k<nkres; k++) nkstp.push_back(1);
    for(unsigned int k=0; k<nkbck; k++) nkstp.push_back(kfact);

    new_kmesh.push_back(kmesh[0]);
    unsigned int kloc=0;
    for(unsigned int k=0; k<nkstp.size(); k++) {
      kloc += nkstp[k];
      new_kmesh.push_back( kmesh.at(kloc) );
    }
  }
  else {
    kmesh = {0.0,0.0};
    nktot = 1;
    new_kmesh = {0.0,0.0};
    nkstp = {1};
  }

  // compute new material and density vectors
  cjsoft::NDArray<int,4>::shaper_type mshaper;
  cjsoft::NDArray<int,4> new_materials;
  new_materials.Reshape( mshaper[nitot][njtot][nktot][nmxsp], cjsoft::NDArray<int,4>::COLMAJOR );

  cjsoft::NDArray<double,4>::shaper_type dshaper;
  cjsoft::NDArray<double,4> new_densities;
  new_densities.Reshape( dshaper[nitot][njtot][nktot][nmxsp], cjsoft::NDArray<double,4>::COLMAJOR );

  // loop over new coarsened mesh
  unsigned int new_nmxsp = 0;
  // unsigned int istart = 0, jstart = 0, kstart = 0;
  unsigned int istart = 0;
  for(unsigned int i=0; i<nitot; i++) {
    unsigned int jstart = 0;
    for(unsigned int j=0; j<njtot; j++) {
      unsigned int kstart = 0;
      for(unsigned int k=0; k<nktot; k++) {
        double new_vol = CalcVolume(new_imesh[i], new_imesh[i+1],
                                    new_jmesh[j], new_jmesh[j+1],
                                    new_kmesh[k], new_kmesh[k+1]);

        // loop over old mesh
        std::vector<int> new_mats;
        std::vector<double> new_dens;
        for( unsigned int ii=istart; ii<istart+nistp[i]; ii++) {
          for( unsigned int jj=jstart; jj<jstart+njstp[j]; jj++) {
            for( unsigned int kk=kstart; kk<kstart+nkstp[k]; kk++) {
              double old_vol = CalcVolume(imesh[ii], imesh[ii+1],
                                          jmesh[jj], jmesh[jj+1],
                                          kmesh[kk], kmesh[kk+1]);

              // loop over materials in old mesh
              for(unsigned int mm=0; mm<static_cast<unsigned int>(m_nmxsp); mm++) {
                int mnum = m_materials[ii][jj][kk][mm];
                double mden = m_densities[ii][jj][kk][mm] * old_vol / new_vol;

                // search for mnum in new_mats
                unsigned int m;
                for(m=0; m<new_mats.size(); m++) {
                  if( new_mats[m] == mnum ) break;
                }

                if( m == new_mats.size() ) {
                  new_mats.push_back(mnum);
                  new_dens.push_back(mden);
                }
                else {
                  new_dens[m] += mden;
                }
              }
            }
          }
        }

        // sort materials by decreasing density
        for(unsigned int s1=0; s1<new_mats.size()-1; s1++) {
          for(unsigned int s2=s1+1; s2<new_mats.size(); s2++) {
            if( new_dens[s1] < new_dens[s2] ) {
              std::swap(new_mats[s1],new_mats[s2]);
              std::swap(new_dens[s1],new_dens[s2]);
            }
          }
        }

        if( static_cast<int>(new_mats.size()) > new_nmxsp )
          new_nmxsp = new_mats.size();

        //store new materials
        for( unsigned int m=0; m<std::min(nmxsp,static_cast<unsigned int>(new_mats.size())); m++ ) {
          new_materials[i][j][k][m] = new_mats[m];
          new_densities[i][j][k][m] = new_dens[m];
        }

        kstart += nkstp[k];
      }
      jstart += njstp[j];
    }
    istart += nistp[i];
  }

  m_materials = new_materials;
  m_densities = new_densities;

  if( void_0_1 ) {
    for(unsigned int i=0; i<nitot; i++) {
      for(unsigned int j=0; j<njtot; j++) {
        for(unsigned int k=0; k<nktot; k++) {
          for(unsigned int m=0; m<nmxsp; m++) {
            if( m_materials[i][j][k][m] == 0 ) {
              m_materials[i][j][k][m] = 1;
              m_densities[i][j][k][m] = 0;
            }
          }
        }
      }
    }
  }

  // reset data
  m_icoarse = nitot;
  m_ifine = nitot;
  m_imesh = new_imesh;
  m_iints = std::vector<int>(nitot,1);

  if( m_geom > CYL1D ) {
    m_jcoarse = njtot;
    m_jfine = njtot;
    m_jmesh = new_jmesh;
    m_jints = std::vector<int>(njtot,1);
  }

  if( m_geom > RT ) {
    m_kcoarse = nktot;
    m_kfine = nktot;
    m_kmesh = new_kmesh;
    m_kints = std::vector<int>(nktot,1);
  }

  m_nmxsp = nmxsp;
}

// compute volume
double Lnk3Dnt::CalcVolume(double ii, double io, double ji, double jo, double ki, double ko) {
  double retval = 0.0;
  switch(m_geom) {
    case SLAB1D:
      retval = (io - ii);
      break;
    case SPH1D:
      retval = 4.0 * pi/3.0 * (std::pow(io,3) - std::pow(ii,3));
      break;
    case CYL1D:
      retval = 2.0 * pi * (std::pow(io,2) - std::pow(ii,2));
      break;
    case XY:
      retval = (io - ii) * (jo - ji);
      break;
    case RZ:
      retval = 2.0 * pi * (std::pow(io,2) - std::pow(ii,2)) * (jo - ji);
      break;
    case RT:
      retval = 2.0 * pi * (std::pow(io,2) - std::pow(ii,2)) * (jo - ji);
      break;
    case XYZ:
      retval = (io - ii) * (jo - ji) * (ko - ki);
      break;
    case RZT:
      retval = 2.0 * pi * (std::pow(io,2) - std::pow(ii,2)) * (jo - ji) * (ko - ki);
      break;
  }

  return retval;
}

// read header information
void Lnk3Dnt::ReadHeader() {
  // create header instance
  header head;

  // read the header data
  m_handle.read((char*) &head, sizeof(head));

  // parse out the strings
  std::stringstream name;
  std::stringstream user;
  std::stringstream version;

  for(int i=0; i<8; i++) {
    name << head.cbuf[i];
    user << head.cbuf[i+8];
    version << head.cbuf[i+16];
  }

  // assign header values to class members
  m_name         =  name.str();
  m_user         =  user.str();
  m_version      =  version.str();
  m_dblparam     =  head.dblparam;
  m_geom         =  (Lnk3DntGeom) head.geom;
  m_nzone        =  head.nzone;
  for(int i=0; i<2; i++) {
    m_reserved[i] = head.reserved00_01[i];
  }
  m_icoarse      =  head.icoarse;
  m_jcoarse      =  head.jcoarse;
  m_kcoarse      =  head.kcoarse;
  m_ifine        =  head.ifine;
  m_jfine        =  head.jfine;
  m_kfine        =  head.kfine;
  for(int i=0; i<13; i++) {
    m_reserved[i+2] = head.reserved02_14[i];
  }
  m_nmxsp        =  head.nmxsp;
  m_reserved[15] =  head.reserved15;
  m_intrec       =  head.intrec;
  m_ilevel       =  head.ilevel;
}

// read the geometry information
void Lnk3Dnt::ReadGeometry() {

  // read i coarse mesh data
  m_imesh.resize(m_icoarse+1);
  m_handle.read((char*) &m_imesh[0], m_imesh.size()*sizeof(double));

  // read j coarse mesh data
  if( m_geom >= XY ) {
    m_jmesh.resize(m_jcoarse+1);
    m_handle.read((char*) &m_jmesh[0], m_jmesh.size()*sizeof(double));
  }

  // read k coarse data
  if( m_geom >= XYZ ) {
    m_kmesh.resize(m_kcoarse+1);
    m_handle.read((char*) &m_kmesh[0], m_kmesh.size()*sizeof(double));
  }

  // read i interval data
  m_iints.resize(m_ifine);
  m_handle.read((char*) &m_iints[0], m_iints.size()*sizeof(int));

  // read j interval data
  if( m_geom >= XY ) {
    m_jints.resize(m_jfine);
    m_handle.read((char*) &m_jints[0], m_jints.size()*sizeof(int));
  }

  // read k interval data
  if( m_geom >= XYZ ) {
    m_kints.resize(m_kfine);
    m_handle.read((char*) &m_kints[0], m_kints.size()*sizeof(int));
  }
}

// read the material information
void Lnk3Dnt::ReadMaterials() {
  // compute the total size of the mesh
  long int isum = 0;
  long int jsum = 0;
  long int ksum = 0;

  for(unsigned int i=0; i<m_iints.size(); i++) {
    isum += m_iints[i];
  }

  for(unsigned int j=0; j<m_jints.size(); j++) {
    jsum += m_jints[j];
  }
  if( jsum == 0 )
    jsum = 1;

  for(unsigned int k=0; k<m_kints.size(); k++) {
    ksum += m_kints[k];
  }
  if( ksum == 0 )
    ksum = 1;

  // compute the total size of the material arrays
  cjsoft::NDArray<int,4>::shaper_type mshaper;
  m_materials.Reshape( mshaper[isum][jsum][ksum][m_nmxsp], cjsoft::NDArray<int,4>::COLMAJOR );

  cjsoft::NDArray<double,4>::shaper_type dshaper;
  m_densities.Reshape( dshaper[isum][jsum][ksum][m_nmxsp], cjsoft::NDArray<double,4>::COLMAJOR );

  // read in the materials for this element
  m_handle.read((char*) &m_materials[0][0][0][0], isum*jsum*ksum*m_nmxsp*sizeof(int));

  // read in the densities for this element
  m_handle.read((char*) &m_densities[0][0][0][0], isum*jsum*ksum*m_nmxsp*sizeof(double));

  // populate the material ids
  std::set<int> m_material_ids(&m_materials[0][0][0][0], &m_materials[0][0][0][0]+isum*jsum*ksum*m_nmxsp);
}

// save a lnk3dnt file
void Lnk3Dnt::Save(const std::string& savename) const {
  // open the file
  std::ofstream ohandle(savename.c_str(), std::ios::binary);

  // create and populate the header
  header head;

  for(int i=0; i<8; i++) {
    head.cbuf[i] = i >= m_name.size() ? ' ' : m_name.at(i);
    head.cbuf[i+8] = i >= m_user.size() ? ' ' : m_user.at(i);
    head.cbuf[i+16] = i >= m_version.size() ? ' ' : m_version.at(i);
  }

  head.dblparam   =  m_dblparam;
  head.geom       =  (int) m_geom;
  head.nzone      =  m_nzone;
  for(int i=0; i<2; i++) {
    head.reserved00_01[i] = m_reserved[i];
  }
  head.icoarse    =  m_icoarse;
  head.jcoarse    =  m_jcoarse;
  head.kcoarse    =  m_kcoarse;
  head.ifine      =  m_ifine;
  head.jfine      =  m_jfine;
  head.kfine      =  m_kfine;
  for(int i=0; i<13; i++) {
    head.reserved02_14[i] = m_reserved[i+2];
  }
  head.nmxsp      =  m_nmxsp;
  head.reserved15 =  m_reserved[15];
  head.intrec     =  m_intrec;
  head.ilevel     =  m_ilevel;

  // write the header
  ohandle.write((char*) &head, sizeof(head));

  // write the geometry information
  // write the coarse mesh in i direction
  ohandle.write((char*) &m_imesh.at(0), m_imesh.size()*sizeof(double));

  // write the coarse mesh in j direction
  if( m_geom >= XY ) {
    ohandle.write((char*) &m_jmesh.at(0), m_jmesh.size()*sizeof(double));
  }

  // write the coarse mesh in j direction
  if( m_geom >= XYZ ) {
    ohandle.write((char*) &m_kmesh.at(0), m_kmesh.size()*sizeof(double));
  }

  // write the fine mesh intervals in i direction
  ohandle.write((char*) &m_iints.at(0), m_iints.size()*sizeof(int));

  // write the fine mesh intervals in j direction
  if( m_geom >= XY ) {
    ohandle.write((char*) &m_jints.at(0), m_jints.size()*sizeof(int));
  }

  // write the fine mesh intervals in k direction
  if( m_geom >= XYZ ) {
    ohandle.write((char*) &m_kints.at(0), m_kints.size()*sizeof(int));
  }

  // write the material information
  // compute the total size of the mesh
  long int isum = 0;
  long int jsum = 0;
  long int ksum = 0;

  for(unsigned int i=0; i<m_iints.size(); i++) {
    isum += m_iints[i];
  }

  for(unsigned int j=0; j<m_jints.size(); j++) {
    jsum += m_jints[j];
  }
  if( jsum == 0 )
    jsum = 1;

  for(unsigned int k=0; k<m_kints.size(); k++) {
    ksum += m_kints[k];
  }
  if( ksum == 0 )
    ksum = 1;

  // write the materials
  ohandle.write((char*) &m_materials[0][0][0][0], isum*jsum*ksum*m_nmxsp*sizeof(int));

  // write the densities
  ohandle.write((char*) &m_densities[0][0][0][0], isum*jsum*ksum*m_nmxsp*sizeof(double));


  // close the file
  ohandle.close();
}

// get string of lnk3dnt information
void Lnk3Dnt::Load(std::string filename) {
  // open the file
  m_handle.open(filename.c_str(), std::ios::binary);

  // read the header information
  ReadHeader();
  // read the geometry information
  ReadGeometry();

  // store material location
  m_matpos = m_handle.tellg();
  // store density location
  long unsigned int offset = m_ifine * m_jfine * m_kfine * m_nmxsp * sizeof(int);
  m_denpos = m_matpos + std::streamoff(offset);
}

std::string l3dinfo(const std::vector<std::string>& files, const bool full) {
  std::stringstream ss;

  for(std::vector<std::string>::const_iterator fit=files.begin(); fit != files.end(); fit++) {
    Lnk3Dnt l3d(*fit);

    long int offset=1;

    ss << "Information about LNK3DNT file \'" << *fit << "\':" << std::endl;
    ss << "     name: " << l3d.GetName() << std::endl;
    ss << "     user: " << l3d.GetUser() << std::endl;
    ss << "  version: " << l3d.GetVersion() << std::endl;
    ss << "     geom: " << l3d.GetGeom() << std::endl;
    ss << "    nzone: " << l3d.GetNZone() << std::endl;
    ss << "  icoarse: " << l3d.GetICoarse() << std::endl;
    ss << "  jcoarse: " << l3d.GetJCoarse() << std::endl;
    ss << "  kcoarse: " << l3d.GetKCoarse() << std::endl;
    ss << "    ifine: " << l3d.GetIFine() << std::endl;
    ss << "    jfine: " << l3d.GetJFine() << std::endl;
    ss << "    kfine: " << l3d.GetKFine() << std::endl;
    ss << "    nmxsp: " << l3d.GetNMXSP() << std::endl;
    ss << std::endl;

    std::vector<double> imesh = l3d.GetIMesh();
    std::vector<double> jmesh = l3d.GetJMesh();
    std::vector<double> kmesh = l3d.GetKMesh();

    ss << std::setprecision(5) << std::scientific;
    if( ! imesh.empty() ) {
      ss << "  (imin,imax): (" << std::setw(12) << imesh.front() << ","
                               << std::setw(12) << imesh.back()  << ")" << std::endl;
      offset *= l3d.GetIFine();
    }
    if( ! jmesh.empty() ) {
      ss << "  (jmin,jmax): (" << std::setw(12) << jmesh.front() << ","
                               << std::setw(12) << jmesh.back()  << ")" << std::endl;
      offset *= l3d.GetJFine();
    }
    if( ! kmesh.empty() ) {
      ss << "  (kmin,kmax): (" << std::setw(12) << kmesh.front() << ","
                               << std::setw(12) << kmesh.back()  << ")" << std::endl;
      offset *= l3d.GetKFine();
    }
    offset *= l3d.GetNMXSP();

    ss << std::endl;

    if( full ) {
      l3d.ReadMaterials();

      cjsoft::NDArray<int,4> mats = l3d.GetMaterials();

      int* mstart = &mats[0][0][0][0];

      std::set<int> mat_ids(mstart, mstart+offset);
      ss << "  Material ID Numbers in LNK3DNT file:" << std::endl;
      for(std::set<int>::const_iterator it=mat_ids.begin(); it!= mat_ids.end(); it++) {
        ss << "    " << *it << std::endl;
      }
    }
  }

  return ss.str();
}

} // end namespace mcnptools
