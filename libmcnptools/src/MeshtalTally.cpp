#include "mcnptools/MeshtalTally.hpp"

#include <cmath>

namespace mcnptools {

// constructor
MeshtalTally::MeshtalTally() {}

double MeshtalTally::GetValue(int i, int j, int k, int e, int t) {
  int tmp_e = e;
  if( tmp_e == -1 ) {
    tmp_e = 0;

    if( m_e.size() > 0 )
      tmp_e = m_e.size()-1;

    if( m_etot )
      tmp_e += 1;
  }

  int tmp_t = t;
  if( tmp_t == -1 ) {
    tmp_t = 0;

    if( m_t.size() > 0 )
      tmp_t = m_t.size()-1;

    if( m_ttot )
      tmp_t += 1;
  }

  return m_data[i][j][k][tmp_e][tmp_t][0];
}

// getError
double MeshtalTally::GetError(int i, int j, int k, int e, int t) {
  int tmp_e = e;
  if( tmp_e == -1 ) {
    tmp_e = 0;

    if( m_e.size() > 0 )
      tmp_e = m_e.size()-1;

    if( m_etot )
      tmp_e += 1;
  }

  int tmp_t = t;
  if( tmp_t == -1 ) {
    tmp_t = 0;

    if( m_t.size() > 0 )
      tmp_t = m_t.size()-1;

    if( m_ttot )
      tmp_t += 1;
  }

  return m_data[i][j][k][tmp_e][tmp_t][1];
}

// getVolume
double MeshtalTally::GetVolume(int i, int j, int k) {
  return m_volumes[i][j][k];
}

// convert to string
std::string MeshtalTally::SummaryString() {
  std::stringstream ss;
  ss << *this;
  return ss.str();
}

// ostream output
std::ostream& operator<<(std::ostream& os, MeshtalTally& mt) {
  // header line
  os << "Mesh Tally " << mt.m_id << std::endl;

  // type
  std::string ttype;
  if( mt.m_type == MeshtalTally::XYZ ) {
    ttype = "XYZ";
  }
  else if(mt.m_type == MeshtalTally::RZT) {
    ttype = "RZT";
  }
  os << "  type = " << ttype << std::endl;

  // i, j, and k bins
  os << "  # i bins = " << mt.m_xr.size() << std::endl;
  os << "  # j bins = " << mt.m_yz.size() << std::endl;
  os << "  # k bins = " << mt.m_zt.size() << std::endl;

  // e bins
  os << "  # energy bins = " << mt.m_e.size() << ", total = " << (mt.m_etot ? "yes" : "no") << std::endl;

  // t bins
  os << "  # time bins = " << mt.m_t.size() << ", total = " << (mt.m_ttot ? "yes" : "no") << std::endl;

  return os;
}

MeshtalTally& MeshtalTally::operator&=(const MeshtalTally& tal) {
  // ensure matching XR bin structure
  bool conforming = true;
  if( m_xr.size() != tal.m_xr.size() ) {
    conforming = false;
  }
  for(unsigned int i=0; i<m_xr.size() && conforming; i++) {
    if( m_xr.at(i) != tal.m_xr.at(i) ) {
      conforming = false;
    }
  }
  if( ! conforming ) {
    std::stringstream ss;
    ss << "meshtal tally i-dimenstion bins are nonconforming";
    throw McnpToolsException(ss.str());
  }

  // ensure matching YZ bin structure
  conforming = true;
  if( m_yz.size() != tal.m_yz.size() ) {
    conforming = false;
  }
  for(unsigned int i=0; i<m_yz.size() && conforming; i++) {
    if( m_yz.at(i) != tal.m_yz.at(i) ) {
      conforming = false;
    }
  }
  if( ! conforming ) {
    std::stringstream ss;
    ss << "meshtal tally j-dimension bins are nonconforming";
    throw McnpToolsException(ss.str());
  }

  // ensure matching ZT bin structure
  conforming = true;
  if( m_zt.size() != tal.m_zt.size() ) {
    conforming = false;
  }
  for(unsigned int i=0; i<m_zt.size() && conforming; i++) {
    if( m_zt.at(i) != tal.m_zt.at(i) ) {
      conforming = false;
    }
  }
  if( ! conforming ) {
    std::stringstream ss;
    ss << "meshtal tally k-dimension bins are nonconforming";
    throw McnpToolsException(ss.str());
  }

  // ensure matching energy bin structure
  conforming = true;
  if( m_e.size() != tal.m_e.size() ) {
    conforming = false;
  }
  for(unsigned int i=0; i<m_e.size() && conforming; i++) {
    if( m_e.at(i) != tal.m_e.at(i) ) {
      conforming = false;
    }
  }
  if( m_etot != tal.m_etot ) {
    conforming = false;
  }
  if( ! conforming ) {
    std::stringstream ss;
    ss << "meshtal tally energy bins are nonconforming";
    throw McnpToolsException(ss.str());
  }

  // ensure matching energy bin structure
  conforming = true;
  if( m_t.size() != tal.m_t.size() ) {
    conforming = false;
  }
  for(unsigned int i=0; i<m_t.size() && conforming; i++) {
    if( m_t.at(i) != tal.m_t.at(i) ) {
      conforming = false;
    }
  }
  if( m_ttot != tal.m_ttot ) {
    conforming = false;
  }
  if( ! conforming ) {
    std::stringstream ss;
    ss << "meshtal tally time bins are nonconforming";
    throw McnpToolsException(ss.str());
  }

  for(unsigned int i=0; i<m_xr.size(); i++) {
    for(unsigned int j=0; j<m_yz.size(); j++) {
      for(unsigned int k=0; k<m_zt.size(); k++) {
        for(unsigned int e=0; e<(m_e.size() + (m_etot ? 1 : 0)); e++) {
          for(unsigned int t=0; t<(m_t.size() + (m_ttot ? 1 : 0)); t++) {
            const double m1 =     m_data[i][j][k][e][t][0];
            const double m2 = tal.m_data[i][j][k][e][t][0];
            const double r1 =     m_data[i][j][k][e][t][1];
            const double r2 = tal.m_data[i][j][k][e][t][1];

            const double val1 = m1 * m_nps;
            const double val2 = m2 * tal.m_nps;

            const double val = (val1 + val2) / (m_nps + tal.m_nps);

            const double err1 = m_nps * std::pow(m1,2) * (std::pow(r1,2)*(m_nps-1)+1.0);
            const double err2 = tal.m_nps * std::pow(m2,2) * (std::pow(r2,2)*(tal.m_nps-1)+1.0);

            double err = 0.0;
            if( (err1+err2) / (m_nps + tal.m_nps) - std::pow(val,2) > 0.0 ) {
              err = std::sqrt( ((err1+err2)/(m_nps + tal.m_nps) - std::pow(val,2)) / (m_nps + tal.m_nps - 1) ) / val;
            }

            m_data[i][j][k][e][t][0] = val;
            m_data[i][j][k][e][t][1] = err;
          }
        }
      }
    }
  }

  m_nps += tal.m_nps;

  return (*this);
}

void MeshtalTally::WriteToMeshtal(std::ostream& os) const {
  // write tally header
  os << " Mesh Tally Number " << std::setw(9) << std::fixed << std::right << m_id << std::endl;
  os << std::endl;

  // write i,j,k boundary info
  os << " Tally bin boundaries:" << std::endl;
  std::string ichar, jchar, kchar;
  std::string ivar, jvar, kvar;
  int ipres, jpres, kpres;
  switch( m_type ) {
    case XYZ:
      ichar = "X"; ivar = "X direction:"; ipres=2;
      jchar = "Y"; jvar = "Y direction:"; jpres=2;
      kchar = "Z"; kvar = "Z direction:"; kpres=2;
      break;
    case RZT:
      ichar = "R"; ivar = "R direction:"; ipres=2;
      jchar = "Z"; jvar = "Z direction:"; jpres=2;
      kchar = "Th"; kvar = "Theta direction (revolutions):"; kpres=3;
      break;
  }
  os << "    " << ivar;
  for(unsigned int i=0; i<m_xr_bounds.size(); i++) {
    os << std::setw(10) << std::setprecision(ipres) << std::fixed << m_xr_bounds.at(i);
  }
  os << std::endl;
  os << "    " << jvar;
  for(unsigned int j=0; j<m_yz_bounds.size(); j++) {
    os << std::setw(10) << std::setprecision(jpres) << std::fixed << m_yz_bounds.at(j);
  }
  os << std::endl;
  os << "    " << kvar;
  for(unsigned int k=0; k<m_zt_bounds.size(); k++) {
    os << std::setw(10) << std::setprecision(kpres) << std::fixed << m_zt_bounds.at(k);
  }
  os << std::endl;

  // write time bin info
  bool write_times = true;
  unsigned int tstart = 0;
  unsigned int tlast = m_t_bounds.size();
  if( m_t_bounds.size() == 2 ) {
    if( m_t_bounds.at(0) == -1e36 && m_t_bounds.at(1) == 1e36 ) {
      write_times = false;
      tlast = 1;
    }
  }
  if( write_times ) {
    os << "    " << "Time bin boundaries:  ";
    for(unsigned int t=0; t<m_t_bounds.size(); t++) {
      os << std::setw(9) << std::setprecision(2) << std::scientific << m_t_bounds.at(t);
    }
    os << std::endl;
  }

  // write energy bin info
  bool write_energies = true;
  unsigned int estart = 0;
  unsigned int elast = m_e_bounds.size();
  if( m_e_bounds.size() == 2 ) {
    if( m_e_bounds.at(1) == 1e36 ) {
      write_energies = false;
      elast = 1;
    }
  }
  os << "    " << "Energy bin boundaries:";
  for(unsigned int e=0; e<m_e_bounds.size(); e++) {
    os << std::setw(9) << std::setprecision(2) << std::scientific << m_e_bounds.at(e);
  }
  os << std::endl;
  os << std::endl;

  if( write_energies )
    os << std::setw(9) << std::right << "Energy" << "  ";
  if( write_times )
    os << std::setw(10) << std::right << "Time" << " ";
  os << std::setw(9) << std::right << ichar;
  os << std::setw(10) << std::right << jchar;
  os << std::setw(10) << std::right << kchar;
  os << std::setw(11) << std::right << "Result";
  os << std::setw(14) << std::right << "Rel Error";
  os << std::setw(11) << std::right << "Volume";
  os << std::setw(14) << std::right << "Rslt * Vol";
  os << std::endl;

  for(unsigned int e=estart; e<elast; e++) {
    for(unsigned int t=tstart; t<tlast; t++) {
      for(unsigned int i=0; i<m_xr.size(); i++) {
        for(unsigned int j=0; j<m_yz.size(); j++) {
          for(unsigned int k=0; k<m_zt.size(); k++) {
            if( write_energies ) {
              std::stringstream ss;
              if( m_etot && e == m_e_bounds.size()-1 ) {
                ss << "   Total   ";
              }
              else {
                ss << std::setw(11) << std::setprecision(3) << std::scientific << m_e.at(e);
              }
              os << ss.str();
            }
            if( write_times ) {
              std::stringstream ss;
              if( m_ttot && t == m_t_bounds.size()-1 ) {
                ss << "   Total   ";
              }
              else {
                ss << std::setw(11) << std::setprecision(3) << std::scientific << m_t.at(t);
              }
              os << ss.str();
            }
            os << std::setw(11) << std::fixed << std::setprecision(3) << m_xr.at(i);
            os << std::setw(10) << std::fixed << std::setprecision(3) << m_yz.at(j);
            os << std::setw(10) << std::fixed << std::setprecision(3) << m_zt.at(k);
            os << std::setw(12) << std::scientific << std::setprecision(5) << m_data[i][j][k][e][t][0];
            os << std::setw(12) << std::scientific << std::setprecision(5) << m_data[i][j][k][e][t][1];
            os << std::setw(12) << std::scientific << std::setprecision(5) << m_volumes[i][j][k];
            os << std::setw(12) << std::scientific << std::setprecision(5) << m_data[i][j][k][e][t][0] * m_volumes[i][j][k];
            os << std::endl;
          }
        }
      }
    }
  }

}

MeshtalTally operator&(const MeshtalTally& t1, const MeshtalTally& t2) {
  MeshtalTally retval = t1;
  retval &= t2;
  return retval;
}

MeshtalTally mergeMeshtalTallies(const MeshtalTally& t1, const MeshtalTally& t2) {
  MeshtalTally retval = t1;
  retval &= t2;
  return retval;
}

} // end namespace mcnptools
