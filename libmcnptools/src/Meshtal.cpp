#include "mcnptools/Constants.hpp"
#include "mcnptools/Meshtal.hpp"
#include "mcnptools/StringOps.hpp"
#include <cmath>
#include <iomanip>

#ifndef __MCNPTOOLS_BOOST_REGEX__

  #include <regex>

  namespace reg = std;

#else

  #include <boost/regex.hpp>

  namespace reg = boost;

#endif

namespace mcnptools {

// constructor
Meshtal::Meshtal(const std::string& fname) {
  m_filename = fname;

  m_handle.open(fname.c_str(), std::ios_base::binary);

  std::string line;

  // read the header line
  cjsoft::stringops::getline(m_handle,line);
  std::vector<std::string> spline;
  cjsoft::stringops::tokenize<std::vector<std::string>,cjsoft::stringops::StringTrim>(line,spline);

  if( spline.size() == 8 ) {
    m_code = spline[0];
    m_version = spline[2];
    m_probid = spline[6] + " " + spline[7];
  }
  else {
    throw McnpToolsException("file " + fname + " does not look like an MCNP meshtal file");
  }

  // read the comment line
  cjsoft::stringops::getline(m_handle,line);
  cjsoft::stringops::trim(line);
  m_comment = line;

  // read the number of histories
  cjsoft::stringops::getline(m_handle,line);
  reg::smatch m;
  reg::regex histmatch("^ Number of histories used for normalizing tallies =\\s+(\\d+\\.\\d+)");
  if( reg::regex_search(line,m,histmatch) ) {
    std::stringstream(m[1]) >> m_nps;
  }
  else {
    throw McnpToolsException("file " + fname + " does not look like an MCNP meshtal file");
  }

  // locate and store tally locations
  std::streamoff loc = m_handle.tellg();
  while( cjsoft::stringops::getline(m_handle,line) ) {
    reg::regex tmatch("^ Mesh Tally Number\\s+(\\d+)");

    if( reg::regex_search(line,m,tmatch) ) {
      int tal;
      std::istringstream(m[1]) >> tal;

      m_tallies.insert( std::pair<int,std::streamoff>(tal,loc) );
    }

    loc = m_handle.tellg();
  }
}

Meshtal::Meshtal(const Meshtal& msht) {
  m_filename = msht.m_filename;
  m_handle.open(m_filename.c_str(), std::fstream::in);

  m_code = msht.m_code;
  m_version = msht.m_version;
  m_probid = msht.m_probid;
  m_nps = msht.m_nps;

  m_comment = msht.m_comment;

  m_tallies = msht.m_tallies;
}

Meshtal Meshtal::operator=(const Meshtal& msht) {
  Meshtal retval(msht);

  return retval;
}

// get the tally list
std::vector<int> Meshtal::GetTallyList(void) const {
  std::vector<int> retval;

  for( std::map<int,std::streamoff>::const_iterator it = m_tallies.begin(); it != m_tallies.end(); it++ ) {
    retval.push_back(it->first);
  }

  return retval;
}

// get a meshtal tally
MeshtalTally Meshtal::GetTally(const int& tnum) const {
  if( m_tallies.find(tnum) == m_tallies.end() ) {
    std::stringstream ss;
    ss << "tally " << tnum << " not found in MESHTAL file \'" << m_filename << "\'" << std::endl;
    throw McnpToolsException( ss.str() );
  }

  // clear the handle b/c we probably reached EOF
  m_handle.clear();
  // seek to the tally location to begin reading
  m_handle.seekg( m_tallies.at(tnum) );

  // begin filling the MeshtalTally
  MeshtalTally ret;

  //set the tally id
  ret.m_id = tnum;

  // set the nps
  ret.m_nps = m_nps;

  std::string line;
  reg::smatch m;

  reg::regex imatch("^\\s+(X|R) direction:(.+)");
  reg::regex ymatch("^\\s+(Y) direction:(.+)");
  reg::regex zmatch("^\\s+(Z) direction:(.+)");
  reg::regex thmatch("^\\s+(Theta) direction \\(revolutions\\):(.+)");
  reg::regex ematch("^\\s+Energy bin boundaries:(.+)");
  reg::regex tmatch("^\\s+Time bin boundaries:(.+)");
  reg::regex dmatch("Result\\s+Rel Error");

  reg::regex talmatch("^\\s+Mesh Tally Number");

  // skip "Mesh Tally Number" line
  cjsoft::stringops::getline(m_handle,line);

  cjsoft::stringops::getline(m_handle,line);
  while( m_handle && ! reg::regex_search(line,m,talmatch) ) {

    // find i direction
    if( reg::regex_search(line,m,imatch) ) {
      if( m[1] == "X" ) {
        ret.m_type = MeshtalTally::XYZ;
      }
      else {
        ret.m_type = MeshtalTally::RZT;
      }

      double d;
      std::stringstream ss(m[2]);
      while( ss >> d ) {
        ret.m_xr_bounds.push_back(d);
      }
    }
    // find y direction
    else if( reg::regex_search(line,m,ymatch) ) {
      double d;
      std::stringstream ss(m[2]);
      while( ss >> d ) {
        ret.m_yz_bounds.push_back(d);
      }
    }
    // find z direction
    else if( reg::regex_search(line,m,zmatch) ) {
      double d;
      std::stringstream ss(m[2]);
      if( ret.m_type == MeshtalTally::XYZ ) {
        while( ss >> d ) {
          ret.m_zt_bounds.push_back(d);
        }
      }
      else {
        while( ss >> d ) {
          ret.m_yz_bounds.push_back(d);
        }
      }
    }
    else if( reg::regex_search(line,m,thmatch) ) {
      double d;
      std::stringstream ss(m[2]);
      while( ss >> d ) {
        ret.m_zt_bounds.push_back(d);
      }
    }

    // find e direction
    if( reg::regex_search(line,m,ematch) ) {
      double d;
      std::stringstream ss(m[1]);
      while( ss >> d ) {
        ret.m_e_bounds.push_back(d);
      }
      ret.m_etot = true;
      if( ret.m_e_bounds.size() == 2 ) {
        // if( ret.m_e_bounds[0] == 0e0 && ret.m_e_bounds[1] == 1e36 ) { // only worked for neutrons
        if( ret.m_e_bounds[1] == 1e36 ) {
          ret.m_etot = false;
        }
      }
    }

    // find t direction
    if( reg::regex_search(line,m,tmatch) ) {
      double d;
      std::stringstream ss(m[1]);
      while( ss >> d ) {
        ret.m_t_bounds.push_back(d);
      }
      ret.m_ttot = true;
    }

    // find data block
    if( reg::regex_search(line,m,dmatch) ) {
      // set time bounds if t direction wasn't present
      if( ret.m_t_bounds.size() == 0 ) {
        ret.m_t_bounds.push_back(-1e36);
        ret.m_t_bounds.push_back(1e36);
        ret.m_ttot = false;
      }

      // total hack until sregex_token_iterator is fixed!!!
      std::vector<std::string> dtypes;
      reg::regex rexpr("\\s{2,}");
      reg::sregex_token_iterator dit(line.begin(), line.end(), rexpr, -1);
      while( dit != reg::sregex_token_iterator() ) {
        std::string dtype(*dit);
        if( dtype.size() > 0 )
          dtypes.push_back( dtype );
        dit++;
      }

      // allocate the multidimensional array for storing data
      int ni = ret.m_xr_bounds.size()-1;
      int nj = ret.m_yz_bounds.size()-1;
      int nk = ret.m_zt_bounds.size()-1;
      int ne = ret.m_e_bounds.size()-1 + (ret.m_etot ? 1 : 0);
      int nt = ret.m_t_bounds.size()-1 + (ret.m_ttot ? 1 : 0);

      cjsoft::NDArray<double,6>::shaper_type shaper;
      ret.m_data.Reshape( shaper[ni][nj][nk][ne][nt][2] );

      // size volumes and set volumes to -1.0
      cjsoft::NDArray<double,3>::shaper_type vshaper;
      ret.m_volumes.Reshape( vshaper[ni][nj][nk] );

      for(int i=0; i<ni; i++) {
        for(int j=0; j<nj; j++) {
          for(int k=0; k<nk; k++) {
            ret.m_volumes[i][j][k] = -1.0;
          }
        }
      }

      for( int e=0; e<ne; e++ ) { //energy bins
        for( int t=0; t<nt; t++ ) { //time bins
          for( int i=0; i<ni; i++ ) { //i bins
            for( int j=0; j<nj; j++ ) { // j bins
              for( int k=0; k<nk; k++ ) { // k bins
                cjsoft::stringops::getline(m_handle,line);
                std::vector<std::string> spline;
                cjsoft::stringops::tokenize<std::vector<std::string>,cjsoft::stringops::StringTrim>(line,spline);

                for(unsigned int d=0; d<dtypes.size(); d++) {
                  if( dtypes[d] == "Energy" ) {
                    if( spline[d] != "Total" ) {
                      double eval;
                      std::stringstream(spline[d]) >> eval;
                      if( ret.m_e.size() == 0 || ret.m_e.back() < eval ) {
                        ret.m_e.push_back(eval);
                      }
                    }
                  }
                  else if( dtypes[d] == "Time" ) {
                    if( spline[d] != "Total" ) {
                      double tval;
                      std::stringstream(spline[d]) >> tval;
                      if( ret.m_t.size() == 0 || ret.m_t.back() < tval ) {
                        ret.m_t.push_back(tval);
                      }
                    }
                  }
                  else if( reg::regex_search(dtypes[d],m,reg::regex("^[X|R]$")) ) {
                    double xr;
                    std::stringstream(spline[d]) >> xr;
                    if( ret.m_xr.size() == 0 || ret.m_xr.back() < xr ) {
                      ret.m_xr.push_back(xr);
                    }
                  }
                  else if( dtypes[d] == "Y" ) {
                    double yz;
                    std::stringstream(spline[d]) >> yz;
                    if( ret.m_yz.size() == 0 || ret.m_yz.back() < yz ) {
                      ret.m_yz.push_back(yz);
                    }
                  }
                  else if( dtypes[d] == "Z" ) { // must handle cartesian or cylindrical
                    double yzt;
                    std::stringstream(spline[d]) >> yzt;
                    if( ret.m_type == MeshtalTally::XYZ ) {
                      if( ret.m_zt.size() == 0 || ret.m_zt.back() < yzt ) {
                        ret.m_zt.push_back(yzt);
                      }
                    }
                    else {
                      if( ret.m_yz.size() == 0 || ret.m_yz.back() < yzt ) {
                        ret.m_yz.push_back(yzt);
                      }
                    }
                  }
                  else if( dtypes[d] == "Th" ) {
                    double zt;
                    std::stringstream(spline[d]) >> zt;
                    if( ret.m_zt.size() == 0 || ret.m_zt.back() < zt ) {
                      ret.m_zt.push_back(zt);
                    }
                  }
                  else if( dtypes[d] == "Result" ) {
                    std::stringstream(spline[d]) >> ret.m_data[i][j][k][e][t][0];
                  }
                  else if( dtypes[d] == "Rel Error" ) {
                    std::stringstream(spline[d]) >> ret.m_data[i][j][k][e][t][1];
                  }
                  else if( dtypes[d] == "Volume" ) {
                    std::stringstream(spline[d]) >> ret.m_volumes[i][j][k];
                  }
                }
              } // k
            } // j
          } // i
        } // t
      } // e

      for(int i=0; i<ni; i++) {
        for(int j=0; j<nj; j++) {
          for(int k=0; k<nk; k++) {
            if( ret.m_volumes[i][j][k] < 0.0 ) {
              if( ret.m_type == MeshtalTally::XYZ ) {
                ret.m_volumes[i][j][k] = (ret.m_xr_bounds[i+1] - ret.m_xr_bounds[i]) *
                                         (ret.m_yz_bounds[j+1] - ret.m_yz_bounds[j]) *
                                         (ret.m_zt_bounds[k+1] - ret.m_zt_bounds[k]);
              }
              else {
                ret.m_volumes[i][j][k] = (std::pow(ret.m_xr_bounds[i+1],2) - std::pow(ret.m_xr_bounds[i],2))/2.0 *
                                         (ret.m_yz_bounds[j+1] - ret.m_yz_bounds[j]) *
                                         (2.0*pi*(ret.m_zt_bounds[k+1] - ret.m_zt_bounds[k]));
              }
            }
          }
        }
      }

    }

    cjsoft::stringops::getline(m_handle,line);
  }

  if( ret.m_e.size() == 0 )
    ret.m_e.push_back(ret.m_e_bounds.back());

  if( ret.m_t.size() == 0 )
    ret.m_t.push_back(ret.m_t_bounds.back());

  return ret;
}

// convert to string
std::string Meshtal::SummaryString() const {
  std::stringstream ss;
  ss << const_cast<Meshtal&>(*this);

  return ss.str();
}

// operator<<
std::ostream& operator<<(std::ostream& sos, Meshtal& mt) {
  sos << "Contents of MESHTAL file " << mt.m_filename << ":" << std::endl;
  sos << "  code: " << mt.GetCode() << std::endl;
  sos << "  version: " << mt.GetVersion() << std::endl;
  sos << "  probid: " << mt.GetProbid() << std::endl;
  sos << "  nps: " << mt.GetNps() << std::endl;
  sos << "  comment: " << mt.GetComment() << std::endl << std::endl;

  sos << "  tallies:";
  std::vector<int> tlist = mt.GetTallyList();
  for(std::vector<int>::iterator it=tlist.begin(); it != tlist.end(); it++) {
    sos << " " << *it;
  }
  sos << std::endl;
  return sos;
}

void Meshtal::WriteHeader(std::ostream& os) const {
  os << std::left << std::setw(6) << m_code
     << " version "
     << std::setw(5) << m_version
     << " ld=" << std::setw(8) << "        "
     << "  probid = " << std::setw(19) << m_probid
     << std::endl;
  os << " " << m_comment << std::endl;
  os << " Number of histories used for normalizing tallies = " << std::setw(16) << std::setprecision(2) << std::fixed << std::right << static_cast<double>(m_nps) << std::endl;
  os << std::endl;
}

Meshtal::Meshtal() {};

Meshtal::Meshtal(const std::string& fname, const std::map<int,std::streamoff>& tallies) {
  m_filename = fname;

  m_handle.open(fname.c_str(), std::fstream::in);

  std::string line;

  // read the header line
  cjsoft::stringops::getline(m_handle,line);
  std::vector<std::string> spline;
  cjsoft::stringops::tokenize<std::vector<std::string>,cjsoft::stringops::StringTrim>(line,spline);

  if( spline.size() == 8 ) {
    m_code = spline[0];
    m_version = spline[2];
    m_probid = spline[6] + " " + spline[7];
  }
  else {
    throw McnpToolsException("file " + fname + " does not look like an MCNP meshtal file");
  }

  // read the comment line
  cjsoft::stringops::getline(m_handle,line);
  cjsoft::stringops::trim(line);
  m_comment = line;

  // read the number of histories
  cjsoft::stringops::getline(m_handle,line);
  reg::smatch m;
  reg::regex histmatch("^ Number of histories used for normalizing tallies =\\s+(\\d+\\.\\d+)");
  if( reg::regex_search(line,m,histmatch) ) {
    std::stringstream(m[1]) >> m_nps;
  }
  else {
    throw McnpToolsException("file " + fname + " does not look like an MCNP meshtal file");
  }

  // store tally locations
  m_tallies = tallies;
}

} // end namespace mcnptools
