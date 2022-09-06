#include "mcnptools/Mctal.hpp"

#include "mcnptools/StringOps.hpp"

#ifndef __MCNPTOOLS_BOOST_REGEX__

  #include <regex>

  namespace reg = std;

#else

  #include <boost/regex.hpp>

  namespace reg = boost;

#endif

namespace mcnptools {

// constructor
Mctal::Mctal(const std::string& fname) {
  m_filename = fname;

  m_handle.open(fname.c_str(), std::ios_base::binary);
  m_kpos = m_handle.tellg();

  std::string line;
  cjsoft::stringops::getline(m_handle,line);

  // read the header line
  std::vector<std::string> spline;
  cjsoft::stringops::tokenize<std::vector<std::string>,cjsoft::stringops::StringTrim>(line,spline);

  if( spline.size() == 3 ) {
    m_code = "mcnp";
    m_version = "?";
    m_probid = "00/00/00 00:00:00";
    std::istringstream(spline[0]) >> m_dump;
    std::istringstream(spline[1]) >> m_nps;
    std::istringstream(spline[2]) >> m_randoms;
  } else if( spline.size() == 7 ) {
    m_code = spline[0];
    m_version = spline[1];
    m_probid = spline[2] + " " + spline[3];
    std::istringstream(spline[4]) >> m_dump;
    std::istringstream(spline[5]) >> m_nps;
    std::istringstream(spline[6]) >> m_randoms;
  } else {
    throw McnpToolsException("file " + fname + " does not look like an MCNP mctal file");
  }

  // get the comment line
  cjsoft::stringops::getline(m_handle,m_comment);

  // read the number of tallies
  cjsoft::stringops::getline(m_handle,line);
  reg::smatch n;
  reg::regex nmatch("^ntal\\s+(\\d+)");
  if( reg::regex_search(line, n, nmatch) ) {
    unsigned int ntal;
    std::stringstream(n[1]) >> ntal;
    while( m_tallies.size() != ntal ) {
      cjsoft::stringops::getline(m_handle,line);
      spline.clear();
      cjsoft::stringops::tokenize<std::vector<std::string>,cjsoft::stringops::StringTrim>(line,spline);

      for(std::vector<std::string>::iterator it=spline.begin(); it!=spline.end(); it++) {
        int tmp;
        std::stringstream(*it) >> tmp;
        m_tallies.push_back(tmp);
      }
    }
  }

  // locate and store tallies and kcode information if present
  std::streamoff loc = m_handle.tellg();
  reg::regex tmatch("^tally\\s+(\\d+)");
  reg::regex kmatch("^kcode");
  while( cjsoft::stringops::getline(m_handle,line) ) {
    reg::smatch m;
    if( reg::regex_search(line, m, tmatch) ) {
      int tal;
      std::istringstream(m[1]) >> tal;
      m_talloc.insert( std::pair<int,std::streamoff>(tal,loc) );
    }
    else if( reg::regex_search(line, m, kmatch) ) {
      m_kpos = loc;
    }

    loc = m_handle.tellg();
  }
}

Mctal::Mctal(const Mctal& mct) {
  m_filename = mct.m_filename;
  m_handle.open(m_filename.c_str(), std::fstream::in);

  m_code    = mct.m_code;
  m_version = mct.m_version;
  m_probid  = mct.m_probid;
  m_dump    = mct.m_dump;
  m_nps     = mct.m_nps;
  m_randoms = mct.m_randoms;
  m_comment = mct.m_comment;

  m_kpos    = mct.m_kpos;
  m_talloc  = mct.m_talloc;
  m_tallies = mct.m_tallies;
}

Mctal Mctal::operator=(const Mctal& mct) {
  Mctal retval(mct);

  return retval;
}

// getTallyList
std::vector<int> Mctal::GetTallyList(void) const {

  std::vector<int> retval;

  std::map<int, std::vector<int> > taltypes;

  for( std::map<int,std::streamoff>::const_iterator it = m_talloc.begin(); it != m_talloc.end(); it++ ) {
    taltypes[it->first % 10].push_back(it->first);
  }

  for( std::map<int, std::vector<int> >::iterator it = taltypes.begin(); it != taltypes.end(); it++ ) {
    for( std::vector<int>::iterator vit = it->second.begin(); vit != it->second.end(); vit++ ) {
      retval.push_back( *vit );
    }
  }

  return retval;
}

// getTally
MctalTally Mctal::GetTally(const int& tnum) const {
  if( m_talloc.find(tnum) == m_talloc.end() ) {
    std::stringstream ss;
    ss << "tally " << tnum << " not found in MCTAL file \'" << m_filename << "\'" << std::endl;
    throw McnpToolsException( ss.str() );
  }

  // clear the handle b/c we probably reached EOF
  m_handle.clear();
  // seek to the tally location to begin reading
  m_handle.seekg( m_talloc.at(tnum) );

  MctalTally tally;
  tally.m_nps = m_nps;
  tally.m_c2r = false;

  // read tally line and store tally id and particle type
  std::string line;
  cjsoft::stringops::getline(m_handle,line);
  reg::regex tmatch("^tally\\s+(\\d+)\\s+(-?\\d+)\\s*(\\d+)?\\s*(\\d+)?");
  reg::smatch m;
  if( reg::regex_search(line,m,tmatch) ) {
    std::stringstream(m[1]) >> tally.m_id;
    std::stringstream(m[2]) >> tally.m_partype;
    if( m.size() > 2 )
      std::stringstream(m[3]) >> tally.m_neetype;
    if( m.size() > 3 )
      std::stringstream(m[4]) >> tally.m_tally_unknown;
  }
  else {
    std::stringstream ss;
    ss << "tally " << tnum << " not found in MCTAL file \'" << m_filename << "\'" << " at location " << m_talloc.at(tnum) << std::endl;
    throw McnpToolsException( ss.str() );
  }

  // read the particle types if needed
  if( tally.m_partype < 0 ) {
    cjsoft::stringops::getline(m_handle,line);
    std::vector<std::string> spline;
    cjsoft::stringops::tokenize<std::vector<std::string>,cjsoft::stringops::StringTrim>(line,spline);
    for(std::vector<std::string>::iterator it=spline.begin(); it!=spline.end(); it++) {
      int tmp;
      std::stringstream(*it) >> tmp;
      tally.m_partypes.push_back(tmp);
    }
  }

  // store any comment line
  std::vector<std::string> comment;
  cjsoft::stringops::getline(m_handle,line);
  reg::regex dmatch("^(f|d|u|s|m|c|r|e|t)(c|t)?\\s+(\\d+)");

  while( m_handle.good() && ! reg::regex_search(line,m,dmatch) ) {
    tally.m_comment.push_back( line );
    cjsoft::stringops::getline(m_handle,line);
  }

  // read the remaining tally data
  int pert;

  bool done = false;
  while(  m_handle.good() && ! done ) {
    reg::regex kmatch("^kcode");
    reg::regex vmatch("^vals(\\s+pert\\s+(\\d+))?");
    reg::regex cmatch("^tfc(\\s+\\d+){9}");

    if( reg::regex_search(line,m,tmatch) || reg::regex_search(line,m,kmatch) ) {
      done = true;
    }
    else if( reg::regex_search(line,m,dmatch) ) {
      char bin = std::stringstream(m[1]).get();
      tally.m_print_bins[bin] = true;

      if( bin == 'r' ) {
        tally.m_c2r = true;
      }

      tally.m_tots[bin] = false;
      if( m[2] == "t" ) {
        tally.m_tots[bin] = true;
      }

      int nb;
      std::stringstream(m[3]) >> nb;
      tally.m_nactualbin[bin] = nb;
      tally.m_nbin[bin] = (nb == 0 ? 1 : nb);

      cjsoft::stringops::getline(m_handle,line);
      reg::regex ematch("^(f|d|u|s|m|c|r|e|t|vals)");

      std::vector<double> ddata;
      while( m_handle.good() && ! reg::regex_search(line,m,ematch) ) {
        std::vector<std::string> sdata;
        cjsoft::stringops::tokenize<std::vector<std::string>,cjsoft::stringops::StringTrim>(line,sdata);
        for( std::vector<std::string>::iterator it=sdata.begin(); it!=sdata.end(); it++ ) {
          double d;
          std::stringstream(*it) >> d;
          ddata.push_back(d);
        }
        cjsoft::stringops::getline(m_handle,line);
      }

      if( ddata.size() == 0 ) {
        tally.m_print_bins[bin] = false;
        for(int i=0; i<tally.m_nbin[bin]; i++) {
          ddata.push_back( (double) i+1 );
        }
      }

      tally.m_bins.insert( std::pair<char,std::vector<double> >(bin,ddata) );

    }
    else if( reg::regex_search(line,m,vmatch) ) {
      pert = 0;
      if( m.length(2) > 0 ) {
        std::stringstream(m[2]) >> pert;
      }

      cjsoft::NDArray<double,9>::shaper_type shaper;
      tally.m_vals[pert].Reshape( shaper[tally.m_nbin['f']]
                                        [tally.m_nbin['d']]
                                        [tally.m_nbin['u']]
                                        [tally.m_nbin['s']]
                                        [tally.m_nbin['m']]
                                        [tally.m_nbin[tally.m_c2r ? 'r' : 'c']]
                                        [tally.m_nbin['e']]
                                        [tally.m_nbin['t']]
                                        [2] );

      std::string tmpstr;
      for( int f=0; f<tally.m_nbin['f']; f++ ) {
        for( int d=0; d<tally.m_nbin['d']; d++ ) {
          for( int u=0; u<tally.m_nbin['u']; u++ ) {
            for( int s=0; s<tally.m_nbin['s']; s++ ) {
              for( int m=0; m<tally.m_nbin['m']; m++ ) {
                for( int c=0; c<tally.m_nbin[tally.m_c2r ? 'r' : 'c']; c++ ) {
                  for( int e=0; e<tally.m_nbin['e']; e++ ) {
                    for( int t=0; t<tally.m_nbin['t']; t++ ) {
                      for( int v=0; v<2; v++ ){
                        m_handle >> tmpstr;
                        tally.m_vals[pert][f][d][u][s][m][c][e][t][v]
                          = cjsoft::stringops::fortran3digitfixup(tmpstr);
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }

      cjsoft::stringops::getline(m_handle,line);
    }
    else if( reg::regex_search(line,m,cmatch) ) {
      // get tfc bins
      std::vector<std::string> spline;
      cjsoft::stringops::tokenize<std::vector<std::string>,cjsoft::stringops::StringTrim>(line,spline);
      std::stringstream(spline[2]) >> tally.m_tfcbins[pert]['f']; tally.m_tfcbins[pert]['f']--;
      std::stringstream(spline[3]) >> tally.m_tfcbins[pert]['d']; tally.m_tfcbins[pert]['d']--;
      std::stringstream(spline[4]) >> tally.m_tfcbins[pert]['u']; tally.m_tfcbins[pert]['u']--;
      std::stringstream(spline[5]) >> tally.m_tfcbins[pert]['s']; tally.m_tfcbins[pert]['s']--;
      std::stringstream(spline[6]) >> tally.m_tfcbins[pert]['m']; tally.m_tfcbins[pert]['m']--;
      if( tally.m_c2r ) {
        std::stringstream(spline[7]) >> tally.m_tfcbins[pert]['r']; tally.m_tfcbins[pert]['r']--;
      }
      else {
        std::stringstream(spline[7]) >> tally.m_tfcbins[pert]['c']; tally.m_tfcbins[pert]['c']--;
      }
      std::stringstream(spline[8]) >> tally.m_tfcbins[pert]['e']; tally.m_tfcbins[pert]['e']--;
      std::stringstream(spline[9]) >> tally.m_tfcbins[pert]['t']; tally.m_tfcbins[pert]['t']--;

      // get tfc data
      cjsoft::stringops::getline(m_handle,line);
      reg::regex ematch("^(tally|kcode|vals)");

      std::string tmpstr;
      while( m_handle.good() && ! reg::regex_search(line,m,ematch) ) {
        std::vector<std::string> spline;
        cjsoft::stringops::tokenize<std::vector<std::string>,cjsoft::stringops::StringTrim>(line,spline);
        double nps;
        std::stringstream(spline[0]) >> tmpstr;
        nps = cjsoft::stringops::fortran3digitfixup(tmpstr);
        tally.m_tfc[pert][MctalTally::TFC_NPS].push_back(nps);
        double val;
        std::stringstream(spline[1]) >> tmpstr;
        val = cjsoft::stringops::fortran3digitfixup(tmpstr);
        tally.m_tfc[pert][MctalTally::TFC_VALUE].push_back(val);
        double err;
        std::stringstream(spline[2]) >> tmpstr;
        err = cjsoft::stringops::fortran3digitfixup(tmpstr);
        tally.m_tfc[pert][MctalTally::TFC_ERROR].push_back(err);
        if( spline.size() == 4 ) {
          double fom;
          std::stringstream(spline[3]) >> tmpstr;
          fom = cjsoft::stringops::fortran3digitfixup(tmpstr);
          tally.m_tfc[pert][MctalTally::TFC_FOM].push_back(fom);
        }
        else {
          tally.m_tfc[pert][MctalTally::TFC_FOM].push_back(0.0);
        }

        cjsoft::stringops::getline(m_handle,line);
      }
    }
    else {
      cjsoft::stringops::getline(m_handle,line);
    }
  }

  return tally;
}

// getKcode
MctalKcode Mctal::GetKcode() {
  if( m_kpos == std::streamoff(0) ) {
    std::stringstream ss;
    ss << "kcode not found in MCTAL file \'" << m_filename << "\'" << std::endl;
    throw McnpToolsException( ss.str() );
  }

  // clear the handle b/c we probably reached EOF
  m_handle.clear();
  // seek to the kcode location
  m_handle.seekg( m_kpos );

  // create a MctalKcode instance
  MctalKcode kc;

  // set the number of histories from the mctal file
  kc.m_nps = m_nps;

  // read the kcode data
  std::string line;
  cjsoft::stringops::getline(m_handle,line);

  // parse the kcode line
  reg::regex kmatch("^kcode\\s+(\\d+)\\s+(\\d+)\\s+(\\d+)");
  reg::smatch m;
  reg::regex_search(line,m,kmatch);

  std::stringstream(m[1]) >> kc.m_cycles;
  std::stringstream(m[2]) >> kc.m_settle;
  std::stringstream(m[3]) >> kc.m_ndat;

  cjsoft::NDArray<double,2>::shaper_type shaper;
  kc.m_kdat.Reshape(shaper[kc.m_cycles][kc.m_ndat]);

  for(unsigned int i=0; i<kc.m_cycles; i++) {
    for(unsigned int j=0; j<kc.m_ndat; j++) {
      m_handle >> kc.m_kdat[i][j];
    }
  }

  return kc;
}

// convert to string
std::string Mctal::SummaryString() const {
  std::stringstream sos;
  sos << const_cast<Mctal&>(*this);

  return sos.str();
}

// ostream output
std::ostream& operator<<(std::ostream& os, Mctal& m) {
  std::stringstream sos;
  sos << "Contents of MCTAL file \'" << m.m_filename << "\'" << std::endl;

  sos << "  code = " << m.m_code << std::endl;
  sos << "  version = " << m.m_version << std::endl;
  sos << "  probid = " << m.m_probid << std::endl;
  sos << "  dump = " << m.m_dump << std::endl;
  sos << "  nps = " << m.m_nps << std::endl;
  sos << "  random #s = " << m.m_randoms << std::endl;
  sos << "  comment = " << m.m_comment << std::endl;

  std::vector<int> tallies = m.GetTallyList();
  for(std::vector<int>::iterator it=tallies.begin(); it!=tallies.end(); it++) {
    MctalTally tally = m.GetTally(*it);
    sos << tally;
  }

  if( m.m_kpos != std::streamoff(0) ) {
    MctalKcode kcode = m.GetKcode();
    sos << kcode;
  }

  os << sos.str();

  return os;
}

void Mctal::WriteHeader(std::ostream& os) const {
  using namespace std;
  // write header line
  os << left;
  os << setw(8) << m_code;

  os << right;
  os << setw(8) << m_version
     << setw(19) << m_probid
     << setw(5) << m_dump << " "
     << setw(15) << m_nps << " "
     << setw(15) << m_randoms << endl;

  // write comment line
  os << m_comment << endl;

  // write the number of tallies
  // int pert_cnt = 0;
  os << "ntal" << setw(6) << m_tallies.size() << endl;
  for(unsigned int i=0; i< m_tallies.size(); i++) {
    os << setw(10) << m_tallies.at(i);
    if( (i+1) % 8 == 0 && (i+1) != m_tallies.size() )
      os << endl;
  }
  os << endl;
}

Mctal Mctal::IsolateTally(const int& tally, const std::string& filename) const {
  if( m_talloc.find(tally) == m_talloc.end() ) {
    std::stringstream ss;
    ss << "tally " << tally << " does not exist in mctal file \'" << m_filename << "\'";
    throw McnpToolsException(ss.str());
  }

  Mctal isomctal;

  // set the meta data
  isomctal.m_filename = filename;
  isomctal.m_code = m_code;
  isomctal.m_version = m_version;
  isomctal.m_probid = m_probid;
  isomctal.m_dump = m_dump;
  isomctal.m_nps = m_nps;
  isomctal.m_randoms = m_randoms;

  isomctal.m_comment = m_comment;

  isomctal.m_kpos = std::streamoff(0);

  isomctal.m_tallies.push_back( tally );

  // write the mctal file
  std::ofstream isotal(filename.c_str());

  isomctal.WriteHeader(isotal);

  isomctal.m_talloc[tally] = isotal.tellp();

  MctalTally tal = GetTally(tally);

  tal.WriteToMctal(isotal);

  isotal.close();

  isomctal.m_handle.open( filename.c_str() );

  return isomctal;
}

Mctal::Mctal() {};

Mctal::Mctal(const std::string& fname, const std::vector<int>& tallies, const std::map<int,std::streamoff>& talloc, const std::streamoff& kpos) {
  m_filename = fname;

  m_handle.open(fname.c_str(), std::fstream::in);

  std::string line;
  cjsoft::stringops::getline(m_handle,line);

  // read the header line
  std::vector<std::string> spline;
  cjsoft::stringops::tokenize<std::vector<std::string>,cjsoft::stringops::StringTrim>(line,spline);

  if( spline.size() == 3 ) {
    m_code = "mcnp";
    m_version = "?";
    m_probid = "00/00/00 00:00:00";
    std::istringstream(spline[0]) >> m_dump;
    std::istringstream(spline[1]) >> m_nps;
    std::istringstream(spline[2]) >> m_randoms;
  } else if( spline.size() == 7 ) {
    m_code = spline[0];
    m_version = spline[1];
    m_probid = spline[2] + " " + spline[3];
    std::istringstream(spline[4]) >> m_dump;
    std::istringstream(spline[5]) >> m_nps;
    std::istringstream(spline[6]) >> m_randoms;
  } else {
    throw McnpToolsException("file " + fname + " does not look like an MCNP mctal file");
  }

  // get the comment line
  cjsoft::stringops::getline(m_handle,m_comment);

  // store the tallies, tally locations, and kcode location
  m_tallies = tallies,
  m_talloc = talloc;
  m_kpos = kpos;
};

std::map<int,std::streamoff> Mctal::GetTallyLocations() const {
  return m_talloc;
}

std::streamoff Mctal::GetKcodeLocation() const {
  return m_kpos;
}

} // end namespace mcnptools

