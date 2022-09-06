#include "mcnptools/MctalKcode.hpp"
#include <iomanip>
#include <cmath>

namespace mcnptools {

// null constructor
MctalKcode::MctalKcode() {}

// get a value from the kcode data
double MctalKcode::GetValue(int value, int cycle) {
  if( value == -1 ) {
    value = AVG_COMBINED_KEFF;
  }

  if( cycle < 0 || cycle >= (int) m_cycles ) {
    cycle = m_cycles - 1;
  }

  return m_kdat[cycle][value];
  // return m_kdat.at(0)[cycle][value];
}

// statistically average two kcode data sets
MctalKcode& MctalKcode::operator&=(const MctalKcode& kc) {
  const unsigned int flat_sum = 0;
  const unsigned int average  = 1;
  const unsigned int std_avg  = 2;
  // check that the kcode data is conformal
  if( kc.m_cycles != m_cycles || kc.m_settle != m_settle || kc.m_ndat != m_ndat ) {
    std::stringstream ss;
    ss << "kcode data is nonconforming";
    throw McnpToolsException(ss.str());
  }
  
  // combine data using the overall number of histories in each case
  for(unsigned int i=0; i<m_cycles; i++) {
    // temporary copy of kcode data
    std::vector<double> new_kdat(m_ndat);
    for(unsigned int j=0; j<m_ndat; j++) {
      
      double m1 = 0.0;
      double m2 = 0.0;
      double e1 = 0.0;
      double e2 = 0.0;
      unsigned int merge_type = average;
      switch( j ) {
        case AVG_COLLSION_KEFF_STD: // combine standard deviations
        case AVG_ABSORPTION_KEFF_STD:
        case AVG_TRACKLENGTH_KEFF_STD:
        case AVG_COMBINED_KEFF_STD:
        case AVG_COMBINED_KEFF_BCS_STD:
        case COMBINED_PRLT_STD:
        case 22:
        case 24:
        case 34:
          merge_type = std_avg;
          m1 =    m_kdat[i][j-1];
          m2 = kc.m_kdat[i][j-1];
          e1 =    m_kdat[i][j];
          e2 = kc.m_kdat[i][j];
          // m1 =    m_kdat.at(0)[i][j-1];
          // m2 = kc.m_kdat.at(0)[i][j-1];
          // e1 =    m_kdat.at(0)[i][j];
          // e2 = kc.m_kdat.at(0)[i][j];
          break;

        case CYCLE_NPS:
          merge_type = flat_sum;
          m1 =    m_kdat[i][j];
          m2 = kc.m_kdat[i][j];
          // m1 =    m_kdat.at(0)[i][j];
          // m2 = kc.m_kdat.at(0)[i][j];
          break;

        default: // only basic averages
          merge_type = average;
          m1 =    m_kdat[i][j];
          m2 = kc.m_kdat[i][j];
          // m1 =    m_kdat.at(0)[i][j];
          // m2 = kc.m_kdat.at(0)[i][j];
          break;
      }

      switch(merge_type) {
        case flat_sum:
          new_kdat[j] = m1 + m2;
          break;
        
        case average:
          new_kdat[j] = (m1 * m_nps + m2 * kc.m_nps) / (m_nps + kc.m_nps);
          break;

        case std_avg:
          const double m = (m1 * m_nps + m2 * kc.m_nps) / (m_nps + kc.m_nps);
          const double t1 =    m_nps*std::pow(m1,2)*(std::pow(e1,2)*(   m_nps-1)+1.0);
          const double t2 = kc.m_nps*std::pow(m2,2)*(std::pow(e2,2)*(kc.m_nps-1)+1.0);
          new_kdat[j] = 0.0;
          if( (t1+t2)/(m_nps+kc.m_nps) - std::pow(m,2) > 0.0 )
            new_kdat[j] = std::sqrt( ((t1+t2)/(m_nps+kc.m_nps) - std::pow(m,2))/(m_nps+kc.m_nps-1.0) );
          break;
      }
    }
    
    // copy the new values into data structure
    for(unsigned int j=0; j<m_ndat; j++) {
      m_kdat[i][j] = new_kdat[j];
      // m_kdat.at(0)[i][j] = new_kdat[j];
    }
  }
  m_nps += kc.m_nps;

  return (*this);
}

// str
std::string MctalKcode::SummaryString() {
  std::stringstream ss;
  ss << *this;

  return ss.str();
}

// ostream << overload
std::ostream& operator<<(std::ostream& os, MctalKcode& kc) {
  os << "Kcode" << std::endl;
  os << "  Cycles: " << kc.m_cycles << std::endl;
  os << "  Settle: " << kc.m_settle << std::endl;
  os << "  # Data: " << kc.m_ndat << std::endl;
  os << std::endl;
  os << "  keff (RE): " << kc.GetValue() << " (" << kc.GetValue(MctalKcode::AVG_COMBINED_KEFF_STD) << ")" << std::endl;

  return os;
}

void MctalKcode::WriteToMctal(std::ostream& os) const {
  using namespace std;
  os << "kcode" << setw(10) << m_cycles
                << setw(10) << m_settle
                << setw(10) << m_ndat << endl;
  os << scientific << setprecision(5);
  for(unsigned int i=0; i<m_kdat.Shape()[0]; i++) {
    for(unsigned int j=0; j<m_kdat.Shape()[1]; j++) {
  // for(unsigned int i=0; i<m_kdat.at(0).Shape()[0]; i++) {
  //   for(unsigned int j=0; j<m_kdat.at(0).Shape()[1]; j++) {
      os << setw(12) << m_kdat[i][j];
      // os << setw(12) << m_kdat.at(0)[i][j];
      if( (j+1) % 5 == 0 && (j+1) != m_ndat )
        os << endl;
    }
    os << endl;
  }
}

MctalKcode operator&(const MctalKcode& k1, const MctalKcode& k2) {
  MctalKcode retval = k1;
  retval &= k2;
  return retval;
}

MctalKcode mergeMctalKcodes(const MctalKcode& k1, const MctalKcode& k2) {
  MctalKcode retval = k1;
  retval &= k2;
  return retval;
}

} // end namespace mcnptools
