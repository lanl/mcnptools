#include "mcnptools/MctalTally.hpp"

#include <set>
#include <cmath>

namespace mcnptools {

MctalTally::MctalTally() {}

// return TFC bins
std::vector<int> MctalTally::GetTFCBins(int pert) {
  std::vector<int> retval;

  retval.push_back( m_tfcbins[pert]['f']);
  retval.push_back( m_tfcbins[pert]['d']);
  retval.push_back( m_tfcbins[pert]['u']);
  retval.push_back( m_tfcbins[pert]['s']);
  retval.push_back( m_tfcbins[pert]['m']);
  retval.push_back( m_tfcbins[pert][m_c2r ? 'r' : 'c']);
  retval.push_back( m_tfcbins[pert]['e']);
  retval.push_back( m_tfcbins[pert]['t']);

  return retval;
}

// return a TFC value at a particular entry (default to the last)
double MctalTally::GetTFCValue(const int tfc_value, const int tfc_entry, const int pert) const {
  int entry = (tfc_entry == -1 ? m_tfc.at(pert).at(TFC_NPS).size()-1 : tfc_entry);
  return m_tfc.at(pert).at(tfc_value).at(entry);
};

// return the value using the bin numbers
double MctalTally::GetValue(int f, int d, int u, int s, int m, int c, int e, int t, int pert) {
  EvalBins(f,d,u,s,m,c,e,t,pert);
  return m_vals[pert][f][d][u][s][m][c][e][t][0];
}

// return the relative error using the bin numbers
double MctalTally::GetError(int f, int d, int u, int s, int m, int c, int e, int t, int pert) {
  EvalBins(f,d,u,s,m,c,e,t,pert);
  return m_vals[pert][f][d][u][s][m][c][e][t][1];
}

// return the bins
std::vector<double> MctalTally::GetFBins() const {
  std::vector<double> retval;
  if( m_bins.find('f') != m_bins.end() ) {
    retval = m_bins.at('f');
  }
  else {
    for(int i=1; i<=m_nactualbin.at('f'); i++) {
      retval.push_back(i);
    }
  }

  return retval;
}

std::vector<double> MctalTally::GetDBins() const {
  std::vector<double> retval;
  if( m_bins.find('d') != m_bins.end() ) {
    retval = m_bins.at('d');
  }
  else {
    for(int i=1; i<=m_nactualbin.at('d'); i++) {
      retval.push_back(i);
    }
  }

  return retval;
}

std::vector<double> MctalTally::GetUBins() const {
  std::vector<double> retval;
  if( m_bins.find('u') != m_bins.end() ) {
    retval = m_bins.at('u');
  }
  else {
    for(int i=1; i<=m_nactualbin.at('u'); i++) {
      retval.push_back(i);
    }
  }

  return retval;
}

std::vector<double> MctalTally::GetSBins() const {
  std::vector<double> retval;
  if( m_bins.find('s') != m_bins.end() ) {
    retval = m_bins.at('s');
  }
  else {
    for(int i=1; i<=m_nactualbin.at('s'); i++) {
      retval.push_back(i);
    }
  }

  return retval;
}

std::vector<double> MctalTally::GetMBins() const {
  std::vector<double> retval;
  if( m_bins.find('m') != m_bins.end() ) {
    retval = m_bins.at('m');
  }
  else {
    for(int i=1; i<=m_nactualbin.at('m'); i++) {
      retval.push_back(i);
    }
  }

  return retval;
}

std::vector<double> MctalTally::GetCBins() const {
  std::vector<double> retval;
  char c = m_c2r ? 'r' : 'c';
  if( m_bins.find( c ) != m_bins.end() ) {
    retval = m_bins.at( c );
  }
  else {
    for(int i=1; i<=m_nactualbin.at( c ); i++) {
      retval.push_back(i);
    }
  }

  return retval;
}

std::vector<double> MctalTally::GetEBins() const {
  std::vector<double> retval;
  if( m_bins.find('e') != m_bins.end() ) {
    retval = m_bins.at('e');
  }
  else {
    for(int i=1; i<=m_nactualbin.at('e'); i++) {
      retval.push_back(i);
    }
  }

  return retval;
}

std::vector<double> MctalTally::GetTBins() const {
  std::vector<double> retval;
  if( m_bins.find('t') != m_bins.end() ) {
    retval = m_bins.at('t');
  }
  else {
    for(int i=1; i<=m_nactualbin.at('t'); i++) {
      retval.push_back(i);
    }
  }

  return retval;
}

// statistically merge two tallies
MctalTally& MctalTally::operator&=(const MctalTally& tal) {
  // ensure matching tally bin structure
  for(std::map<char,std::vector<double> >::iterator bit=m_bins.begin(); bit!=m_bins.end(); bit++) {
    std::vector<double> tal_bins = tal.m_bins.at(bit->first);
    bool conforming = true;
    if( bit->second.size() != tal_bins.size() ) {
      conforming = false;
    }
    for(unsigned int i=0; i<bit->second.size() && conforming ; i++) {
      if( tal_bins.at(i) != bit->second.at(i) ) 
        conforming = false;
      }

    if( ! conforming ) {
      std::stringstream ss;
      ss << "mctal tally " << "\'" << bit->first << "\'" << " bins are nonconforming";
      throw McnpToolsException(ss.str());
    }
  }

  // ensure matching perturbations

  std::set<int> perts;
  for(std::map<int,cjsoft::NDArray<double,9> >::iterator pit=m_vals.begin(); pit!=m_vals.end(); pit++) {
    perts.insert(pit->first);
  }

  for(std::map<int,cjsoft::NDArray<double,9> >::const_iterator pit=tal.m_vals.begin(); pit!=tal.m_vals.end(); pit++) {
    perts.insert(pit->first);
  }

  for(std::set<int>::iterator pit=perts.begin(); pit!=perts.end(); pit++) {
    if( m_vals.find(*pit) == m_vals.end() || tal.m_vals.find(*pit) == tal.m_vals.end() ) {
      std::stringstream ss;
      ss << "perturbations are nonconforming";
      throw McnpToolsException(ss.str());
    }
  }

  // merge the values
  for(std::set<int>::iterator pit=perts.begin(); pit!=perts.end(); pit++) {
    for( int f=0; f<m_nbin['f']; f++ ) {
      for( int d=0; d<m_nbin['d']; d++ ) {
        for( int u=0; u<m_nbin['u']; u++ ) {
          for( int s=0; s<m_nbin['s']; s++ ) {
            for( int m=0; m<m_nbin['m']; m++ ) {
              for( int c=0; c<m_nbin[m_c2r ? 'r' : 'c']; c++ ) {
                for( int e=0; e<m_nbin['e']; e++ ) {
                  for( int t=0; t<m_nbin['t']; t++ ) {
                    // boost::array<cjsoft::NDArray<double,9>::index,9> vidx = {{f,d,u,s,m,c,e,t,0}};
                    // boost::array<cjsoft::NDArray<double,9>::index,9> eidx = {{f,d,u,s,m,c,e,t,0}};
                    // const double m1 =      m_vals.at(*pit)(vidx);
                    // const double m2 =  tal.m_vals.at(*pit)(vidx);
                    // const double r1 =      m_vals.at(*pit)(eidx);
                    // const double r2 =  tal.m_vals.at(*pit)(eidx);
                    const double m1 =      m_vals.at(*pit)[f][d][u][s][m][c][e][t][0];
                    const double m2 =  tal.m_vals.at(*pit)[f][d][u][s][m][c][e][t][0];
                    const double r1 =      m_vals.at(*pit)[f][d][u][s][m][c][e][t][1];
                    const double r2 =  tal.m_vals.at(*pit)[f][d][u][s][m][c][e][t][1];

                    const double val1 = m1 * m_nps;
                    const double val2 = m2 * tal.m_nps;

                    const double val = (val1 + val2) / (m_nps + tal.m_nps);

                    const double err1 = m_nps * std::pow(m1,2) * (std::pow(r1,2)*(m_nps-1)+1.0);
                    const double err2 = tal.m_nps * std::pow(m2,2) * (std::pow(r2,2)*(tal.m_nps-1)+1.0);
                    
                    double err = 0.0;
                    if( (err1+err2) / (m_nps + tal.m_nps) - std::pow(val,2) > 0.0 ) {
                      err = std::sqrt( ((err1+err2)/(m_nps + tal.m_nps) - std::pow(val,2)) / (m_nps + tal.m_nps - 1) ) / val;
                    }

                    m_vals[*pit][f][d][u][s][m][c][e][t][0] = val;
                    m_vals[*pit][f][d][u][s][m][c][e][t][1] = err;
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  m_nps += tal.m_nps;

  return (*this);
}

// convert tally to string
std::string MctalTally::SummaryString() {
  std::stringstream sos;
  sos << *this;

  return sos.str();
}

// return ostream of class
std::ostream& operator<<(std::ostream& os, MctalTally& t) {
  std::stringstream sos;
  sos << "Tally" << " " << t.m_id << std::endl;
  
  for(std::vector<std::string>::const_iterator it=t.m_comment.begin(); it!=t.m_comment.end(); it++) {
    sos << "  " + *it << std::endl;
  }

  if( t.m_vals.size() > 1 ) {
    sos << std::setw(39) <<"  number of perturbations = " << std::setw(0) << t.m_vals.size() << std::endl;
  }

  const char cb[] = { 'f', 'd', 'u', 's', 'm', 'c', 'e', 't' };

  for( int i=0; i<8; i++ ) {
    char bin = cb[i];
    std::string keyword;
    if( cb[i] == 'f' ) {
      keyword = "surface/cell";
    }
    if( cb[i] == 'd' ) {
      keyword = "flagged/uncollided";
    }
    if( cb[i] == 'u' ) {
      keyword = "user";
    }
    if( cb[i] == 's' ) {
      keyword = "segment";
    }
    if( cb[i] == 'm' ) {
      keyword = "multiplier";
    }
    if( cb[i] == 'c' ) {
      if( t.m_c2r ) {
        bin = 'r';
        keyword = "radii";
      }
      else {
        keyword = "cosine";
      }
    }
    if( cb[i] == 'e' ) {
      keyword = "energy";
    }
    if( cb[i] == 't' ) {
      keyword = "time";
    }

    std::string yes_or_no = "no";
    if( t.m_tots.at(bin) == true ) {
      yes_or_no = "yes";
    }

    std::stringstream text;
    text << "  number of " << keyword << " bins = " << t.m_nbin.at(bin);

    sos << std::setw(40) << text.str() << std::setw(0) << ", total bin = " << yes_or_no << std::endl;
  }
  
  os << sos.str();

  return os;
}

void MctalTally::WriteToMctal(std::ostream& os) const {
  using namespace std;

  unsigned int count;

  // write the tally line
  os << "tally" << setw(10) << m_id
                << setw(21) << m_partype
                << setw(5)  << m_neetype
                << setw(5)  << m_tally_unknown << endl;

  // write the particle types if needed
  if( m_partype < 0 ) {
    count = 0;
    for(unsigned int i=0; i<m_partypes.size(); i++) {
      if( count == 40 ) {
        os << endl;
        count = 0;
      }
      os << setw(2) << m_partypes.at(i);
    }
    os << endl;
  }

  // write the comment lines
  for(unsigned int i=0; i<m_comment.size(); i++) {
    os << m_comment.at(i) << endl;
  }

  // write the bin lines
  const std::vector<std::string> bincs = {"f","d","u","s","m","c","e","t"};
  for(unsigned int i=0; i<bincs.size(); i++) {
    char b = bincs.at(i).at(0);
    std::string c = bincs.at(i);
    if( c == "c" && m_c2r ) {
      c = "r";
      b = 'r';
    }

    if( m_tots.at( b ) )
      c += "t";

    os << left << setw(2) << c;
    os << right << setw(8) << m_nactualbin.at( b ) << endl;

    if( b == 'f' ) {
      os << fixed << setprecision(1);
    }
    else {
      os << scientific << setprecision(5);
    }

    if( m_bins.find(b) != m_bins.end() && m_print_bins.at(b) ) {
      count = 0;
      for(unsigned int i=0; i<m_bins.at(b).size(); i++) {
        switch(b) {
          case 'f':
            os << fixed << setprecision(0);
            if( m_bins.at(b).at(i) - (int) m_bins.at(b).at(i) > 0 )
              os << setprecision(1);
            if( count == 7 ) {
              os << endl;
              count = 0;
            }
            os << setw(11) << m_bins.at(b).at(i);
            count++;
            break;
          default:
            os << scientific << setprecision(5);
            if( count == 6 ) {
              os << endl;
              count = 0;
            }
            os << setw(13) << m_bins.at(b).at(i);
            count++;
            break;
        }
      }
      os << endl;
    }
  }

  // write the values
  for(std::map<int,cjsoft::NDArray<double,9> >::const_iterator it=m_vals.begin(); it!=m_vals.end(); it++) {
    os << "vals";
    if( it->first != 0 )
      os << " pert " << setw(5) << it->first;
    os << endl;

    count = 0;
    for( int f=0; f<m_nbin.at('f'); f++ ) {
      for( int d=0; d<m_nbin.at('d'); d++ ) {
        for( int u=0; u<m_nbin.at('u'); u++ ) {
          for( int s=0; s<m_nbin.at('s'); s++ ) {
            for( int m=0; m<m_nbin.at('m'); m++ ) {
              for( int c=0; c<m_nbin.at(m_c2r ? 'r' : 'c'); c++ ) {
                for( int e=0; e<m_nbin.at('e'); e++ ) {
                  for( int t=0; t<m_nbin.at('t'); t++ ) {
                    if( count == 4 ) {
                      os << endl;
                      count = 0;
                    }
                    // boost::array<cjsoft::NDArray<double,9>::index,9> vindex = {{f,d,u,s,m,c,e,t,0}};
                    os << scientific << setprecision(5);
                    os << setw(13) << it->second[f][d][u][s][m][c][e][t][0];
                    // os << setw(13) << it->second(vindex);

                    // boost::array<cjsoft::NDArray<double,9>::index,9> rindex = {{f,d,u,s,m,c,e,t,1}};
                    os << fixed << setprecision(4);
                    os << setw(7) << it->second[f][d][u][s][m][c][e][t][1];
                    // os << setw(7) << it->second(rindex);
                    count++;
                  }
                }
              }
            }
          }
        }
      }
    }
    os << endl;

    // write the tfc stuff
    unsigned int ntfc = m_tfc.at(it->first).at(TFC_NPS).size();
    os << "tfc" << setw(5) << ntfc
                << setw(8) << m_tfcbins.at(it->first).at('f') + 1
                << setw(8) << m_tfcbins.at(it->first).at('d') + 1
                << setw(8) << m_tfcbins.at(it->first).at('u') + 1
                << setw(8) << m_tfcbins.at(it->first).at('s') + 1
                << setw(8) << m_tfcbins.at(it->first).at('m') + 1
                << setw(8) << m_tfcbins.at(it->first).at(m_c2r ? 'r' : 'c') + 1
                << setw(8) << m_tfcbins.at(it->first).at('e') + 1
                << setw(8) << m_tfcbins.at(it->first).at('t') + 1 << endl;

    os << scientific << setprecision(5);
    for(unsigned int i=0; i<ntfc; i++) {
      os << setw(11) << (int) m_tfc.at(it->first).at(TFC_NPS).at(i)
         << setw(13) <<  m_tfc.at(it->first).at(TFC_VALUE).at(i)
         << setw(13) <<  m_tfc.at(it->first).at(TFC_ERROR).at(i);
      if( m_tfc.at(it->first).find(TFC_FOM) != m_tfc.at(it->first).end() )
        os << setw(13) <<  m_tfc.at(it->first).at(TFC_FOM).at(i);
        os << endl;
      }
    }

  }

  void MctalTally::EvalBins(int& f, int& d, int& u, int& s, int& m, int& c, 
                            int& e, int& t, int& pert) const {

    using pair_t = std::pair<char,int&>;

    const std::array<pair_t,8> index_map{{
      pair_t('f', f),
      pair_t('d', d),
      pair_t('u', u),
      pair_t('s', s),
      pair_t('m', m),
      pair_t(m_c2r ? 'r' : 'c', c),
      pair_t('e', e),
      pair_t('t', t),
    }};

    for( auto& v : index_map ) {
      v.second = v.second == TFC ? this->m_tfcbins.at(pert).at(v.first) : v.second;
  }
}

MctalTally operator&(const MctalTally& t1, const MctalTally& t2) {
  MctalTally retval = t1;
  retval &= t2;
  return retval;
}

MctalTally mergeMctalTallies(const MctalTally& t1, const MctalTally& t2) {
  MctalTally retval = t1;
  retval &= t2;
  return retval;
}


} // end namespace mcnptools
