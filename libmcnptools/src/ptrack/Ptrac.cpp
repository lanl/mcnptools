#include <cassert>
#include <string>

#include "mcnptools/Ptrac.hpp"

#include "mcnptools/StringOps.hpp"

namespace mcnptools {

const std::vector<std::string> Ptrac::m_lines = {"nps","src1","src2","bnk1","bnk2","sur1","sur2","col1","col2","ter1","ter2"};

Ptrac::Ptrac(const std::string& filename, const unsigned int format):
  m_filename( filename ),
  m_format( static_cast<Ptrac::PtracFormat>(format) ) {

  if( format == Ptrac::HDF5_PTRAC ) {
    m_handle = {};
    auto file_h5 = f5::File(m_filename, 'r');
    m_hdf5_parser = std::make_unique<Ptrac::HDF5Parser>( MakeHDF5PtrackParser(file_h5, "ptrack") );
  }
  else
  {
    if( format == Ptrac::BIN_PTRAC ) {
      m_handle.open(filename.c_str(), std::ifstream::binary);
      if (m_handle.fail()) {
        std::stringstream ss;
        ss << "Failed to open binary PTRAC file " << filename;
        throw McnpToolsException( ss.str() );
      }
    }
    else { //ASCII format 
      m_handle.open(filename.c_str());
      if (m_handle.fail()) {
        std::stringstream ss;
        ss << "Failed to open ASCII PTRAC file " << filename;
        throw McnpToolsException( ss.str() );
      }
    }
    ReadHeader();
  }
}

void Ptrac::ReadHeader() {
  if( m_format == Ptrac::BIN_PTRAC ) {
    int size1, size2;

    // determine file size
    m_handle.seekg(0,std::ios::end);
    int64_t fsize = m_handle.tellg();
    m_handle.seekg(0,std::ios::beg);

    // read the version
    m_handle.read( (char*) &size1, sizeof(int) );

    if( size1 >= fsize || size1 != sizeof(int) ) {
      std::stringstream ss;
      ss << "Failed to read binary PTRAC";
      throw McnpToolsException( ss.str() );
    }

    m_handle.read( (char*) &m_version, size1);
    m_handle.read( (char*) &size2, sizeof(int) );

    if( size1 != size2 || m_version != -1 ) {
      std::stringstream ss;
      ss << "Failed to read binary PTRAC";
      throw McnpToolsException( ss.str() );
    }

    // read the code data 
    m_handle.read( (char*) &size1, sizeof(int) );
    
    char code[8], ver[5], loddat[8], idtm[19];
    m_handle.read( code, sizeof(code) );
    m_code = std::string(code,sizeof(code));
    m_handle.read( ver, sizeof(ver) );
    m_codever = std::string(ver,sizeof(ver));
    m_handle.read( loddat, sizeof(loddat) );
    m_loddat = std::string(loddat,sizeof(loddat));
    m_handle.read( idtm, sizeof(idtm) );
    m_idtm = std::string(idtm,sizeof(idtm));
    cjsoft::stringops::trim(m_idtm);

    m_handle.read( (char*) &size2, sizeof(int) );

    if( size1 != size2 ) {
      std::stringstream ss;
      ss << "Failed to read binary PTRAC";
      throw McnpToolsException( ss.str() );
    }

    // read the comment line
    m_handle.read( (char*) &size1, sizeof(int) );
    assert( size1 == 80 || size1 == 128 );

    m_comment.resize(size1);
    m_handle.read(&m_comment[0], size1*sizeof(char));
    
    m_handle.read( (char*) &size2, sizeof(int) );

    if( size1 != size2 ) {
      std::stringstream ss;
      ss << "Failed to read binary PTRAC";
      throw McnpToolsException( ss.str() );
    }

    // read the keyword entries
    bool done = false;
    std::vector<double> kwent;
    unsigned int nkw = 0;
    unsigned int kwlinecnt = 0;
    while (! done) {
      kwlinecnt++;
      m_handle.read( (char*) &size1, sizeof(int) );

      double buffer[10];
      m_handle.read( (char*) buffer, 10*sizeof(double) );
      if( kwlinecnt == 1 ) {
        nkw = (unsigned int) buffer[0];
        kwent.insert(kwent.end(), &buffer[1], &buffer[1] + 9);
      }
      else {
        kwent.insert(kwent.end(), buffer, buffer + 10);
      }

      unsigned int nkwcnt = 0;
      unsigned int i=0;
      while( i<kwent.size() ) {
        nkwcnt++;
        unsigned int num_ent = (unsigned int) kwent[i];
        i += num_ent+1;
      }

      m_handle.read( (char*) &size2, sizeof(int) );

      if( size1 != size2 ) {
        std::stringstream ss;
        ss << "Failed to read binary PTRAC";
        throw McnpToolsException( ss.str() );
      }

      if( nkwcnt >= nkw )
        done = true;
    }

    // read number data
    m_handle.read( (char*) &size1, sizeof(int) );

    int nnps, ipt, single_double, unused[7];
    int64_t nsrc1, nsrc2, nbnk1, nbnk2, nsur1, nsur2, ncol1, ncol2, nter1, nter2;
    m_handle.read( (char*) &nnps, sizeof(int) );
    m_handle.read( (char*) &nsrc1, sizeof(nsrc1) );
    m_handle.read( (char*) &nsrc2, sizeof(nsrc2) );
    m_handle.read( (char*) &nbnk1, sizeof(nbnk1) );
    m_handle.read( (char*) &nbnk2, sizeof(nbnk2) );
    m_handle.read( (char*) &nsur1, sizeof(nsur1) );
    m_handle.read( (char*) &nsur2, sizeof(nsur2) );
    m_handle.read( (char*) &ncol1, sizeof(ncol1) );
    m_handle.read( (char*) &ncol2, sizeof(ncol2) );
    m_handle.read( (char*) &nter1, sizeof(nter1) );
    m_handle.read( (char*) &nter2, sizeof(nter2) );
    m_handle.read( (char*) &ipt, sizeof(int) );
    m_handle.read( (char*) &single_double, sizeof(int) );
    m_handle.read( (char*) &unused, sizeof(unused) );

    m_nument.insert( std::pair<std::string, int64_t>( "nps", nnps ) );
    m_nument.insert( std::pair<std::string, int64_t>( "src1", nsrc1 ) );
    m_nument.insert( std::pair<std::string, int64_t>( "src2", nsrc2 ) );
    m_nument.insert( std::pair<std::string, int64_t>( "bnk1", nbnk1 ) );
    m_nument.insert( std::pair<std::string, int64_t>( "bnk2", nbnk2 ) );
    m_nument.insert( std::pair<std::string, int64_t>( "sur1", nsur1 ) );
    m_nument.insert( std::pair<std::string, int64_t>( "sur2", nsur2 ) );
    m_nument.insert( std::pair<std::string, int64_t>( "col1", ncol1 ) );
    m_nument.insert( std::pair<std::string, int64_t>( "col2", ncol2 ) );
    m_nument.insert( std::pair<std::string, int64_t>( "ter1", nter1 ) );
    m_nument.insert( std::pair<std::string, int64_t>( "ter2", nter2 ) );

    m_handle.read( (char*) &size2, sizeof(int) );
    
    if( size1 != size2 ) {
      std::stringstream ss;
      ss << "Failed to read binary PTRAC";
      throw McnpToolsException( ss.str() );
    }

    // read data types
    m_handle.read( (char*) &size1, sizeof(int) );

    for(unsigned int i=0; i<m_lines.size(); i++) {
      for(unsigned int j=0; j<m_nument[ m_lines[i] ]; j++) {
        if( m_lines[i] == "nps" ) {
          int64_t tmp;
          m_handle.read( (char*) &tmp, sizeof(tmp) );
          m_datent[ m_lines[i] ].push_back( tmp );
        }
        else {
          int tmp;
          m_handle.read( (char*) &tmp, sizeof(tmp) );
          m_datent[ m_lines[i] ].push_back( tmp );
        }
      }
    }

    m_handle.read( (char*) &size2, sizeof(int) );

    if( size1 != size2 ) {
      std::stringstream ss;
      ss << "Failed to read binary PTRAC";
      throw McnpToolsException( ss.str() );
    }

  }
  else {
    // read the version
    m_handle >> m_version;

    // read the code data, if it is available (prdmp third entry effects this)
    std::string idtm1, idtm2;
    std::string optional_version_line;
    std::getline(m_handle, optional_version_line); // processes to the next line
    std::getline(m_handle, optional_version_line); 
    if (optional_version_line != " ") { 
      std::stringstream ss {optional_version_line};
      ss >> m_code >> m_codever >> m_loddat >> idtm1 >> idtm2;
      m_idtm = idtm1 + " " + idtm2;
    } // else, the above variables are default-constructed empty strings

    // read the comment line
    cjsoft::stringops::getline(m_handle, m_comment); // processes to the next line
    cjsoft::stringops::getline(m_handle, m_comment);

    // read the keyword entries
    bool done = false;
    std::vector<double> kwent;
    unsigned int nkw = 0;
    unsigned int kwlinecnt = 0;
    while (! done) {
      kwlinecnt++;

      if( kwlinecnt == 1 ) {
        double tmp;
        m_handle >> tmp;
        nkw = (unsigned int) tmp;
        for(unsigned int i=1; i<10; i++) {
          m_handle >> tmp;
          kwent.push_back(tmp);
        }
      }
      else {
        double tmp;
        for(unsigned int i=0; i<10; i++) {
          m_handle >> tmp;
          kwent.push_back(tmp);
        }
      }

      unsigned int nkwcnt = 0;
      unsigned int i=0;
      while( i<kwent.size() ) {
        nkwcnt++;
        unsigned int num_ent = (unsigned int) kwent[i];
        i += num_ent+1;
      }

      if( nkwcnt >= nkw )
        done = true;
    }

    // read number data
    int nnps, ipt, single_double, unused[7];
    int64_t nsrc1, nsrc2, nbnk1, nbnk2, nsur1, nsur2, ncol1, ncol2, nter1, nter2;

    m_handle >> nnps >> nsrc1 >> nsrc2 >> nbnk1 >> nbnk2 >> nsur1 >> nsur2 >> ncol1 >> ncol2 >> nter1 >> nter2 >> ipt >> single_double;
    for(unsigned int i=0; i<7; i++) {
      m_handle >> unused[i];
    }

    m_nument.insert( std::pair<std::string, int64_t>( "nps", nnps ) );
    m_nument.insert( std::pair<std::string, int64_t>( "src1", nsrc1 ) );
    m_nument.insert( std::pair<std::string, int64_t>( "src2", nsrc2 ) );
    m_nument.insert( std::pair<std::string, int64_t>( "bnk1", nbnk1 ) );
    m_nument.insert( std::pair<std::string, int64_t>( "bnk2", nbnk2 ) );
    m_nument.insert( std::pair<std::string, int64_t>( "sur1", nsur1 ) );
    m_nument.insert( std::pair<std::string, int64_t>( "sur2", nsur2 ) );
    m_nument.insert( std::pair<std::string, int64_t>( "col1", ncol1 ) );
    m_nument.insert( std::pair<std::string, int64_t>( "col2", ncol2 ) );
    m_nument.insert( std::pair<std::string, int64_t>( "ter1", nter1 ) );
    m_nument.insert( std::pair<std::string, int64_t>( "ter2", nter2 ) );

    // read data types

    for(unsigned int i=0; i<m_lines.size(); i++) {
      for(unsigned int j=0; j<m_nument[ m_lines[i] ]; j++) {
        if( m_lines[i] == "nps" ) {
          int64_t tmp;
          m_handle >> tmp;
          m_datent[ m_lines[i] ].push_back( tmp );
        }
        else {
          int tmp;
          m_handle >> tmp;
          m_datent[ m_lines[i] ].push_back( tmp );
        }
      }
    }
  }
}

PtracHistory Ptrac::ReadHistory() {
  int size1, size2;

  PtracHistory hist;

  // read the nps line
  double next_event_type;
  if( m_format == Ptrac::BIN_PTRAC ) ReadValue(size1);

  PtracNps nps;
  for(unsigned int i=0; i<m_nument["nps"]; i++) {
    int64_t tmp;

    ReadValue(tmp);

    if( ! m_handle.good() )
      return hist;

    switch( m_datent["nps"][i] ) {
      case Ptrac::NPS:
        nps.m_nps = tmp;
        break;
      case Ptrac::FIRST_EVENT_TYPE:
        next_event_type = tmp;
        break;
      case Ptrac::NPSCELL:
        nps.m_cell = tmp;
        break;
      case Ptrac::NPSSURFACE:
        nps.m_surface = tmp;
        break;
      case Ptrac::TALLY:
        nps.m_tally = tmp;
        break;
      case Ptrac::VALUE:
        nps.m_tally = tmp;
        break;
    }
  }

  if( m_format == Ptrac::BIN_PTRAC ) {
    ReadValue(size2);

    if( size1 != size2 ) {
      std::stringstream ss;
      ss << "Failed to read binary PTRAC";
      throw McnpToolsException( ss.str() );
    }
  }

  hist.m_nps = nps;

  // read the events
  while( (int) next_event_type != Ptrac::LST ) {
    int bnk_type = std::abs(static_cast<int>(next_event_type)) % 1000;
    next_event_type = std::abs(static_cast<int>(next_event_type)) - bnk_type;

    std::string typestr;
    switch( (int) next_event_type ) {
      case Ptrac::SRC:
        typestr = "src";
        break;
      case Ptrac::BNK:
        typestr = "bnk";
        break;
      case Ptrac::SUR:
        typestr = "sur";
        break;
      case Ptrac::COL:
        typestr = "col";
        break;
      case Ptrac::TER:
        typestr = "ter";
        break;
    }

    PtracEvent event;
    event.m_type = next_event_type;
    event.m_bnktype = bnk_type;
  
    std::vector<int> all_data_types;
    all_data_types.insert(all_data_types.end(), m_datent[typestr + "1"].begin(), m_datent[typestr + "1"].end());
    all_data_types.insert(all_data_types.end(), m_datent[typestr + "2"].begin(), m_datent[typestr + "2"].end());
  
    if( m_format == Ptrac::BIN_PTRAC) ReadValue(size1);

    for(unsigned int i=0; i<all_data_types.size(); i++) {
      double tmp;

      ReadValue(tmp);

      switch( all_data_types[i] ) {
        case Ptrac::NEXT_EVENT_TYPE:
          next_event_type = tmp;
          break;
        case Ptrac::NODE:
          event.m_data[Ptrac::NODE] = tmp;
          break;
        case Ptrac::NSR:
          event.m_data[Ptrac::NSR] = tmp;
          break;
        case Ptrac::ZAID:
          event.m_data[Ptrac::ZAID] = tmp;
          break;
        case Ptrac::RXN:
          event.m_data[Ptrac::RXN] = tmp;
          break;
        case Ptrac::SURFACE:
          event.m_data[Ptrac::SURFACE] = tmp;
          break;
        case Ptrac::ANGLE:
          event.m_data[Ptrac::ANGLE] = tmp;
          break;
        case Ptrac::TERMINATION_TYPE:
          event.m_data[Ptrac::TERMINATION_TYPE] = tmp;
          break;
        case Ptrac::BRANCH:
          event.m_data[Ptrac::BRANCH] = tmp;
          break;
        case Ptrac::PARTICLE:
          event.m_data[Ptrac::PARTICLE] = tmp;
          break;
        case Ptrac::CELL:
          event.m_data[Ptrac::CELL] = tmp;
          break;
        case Ptrac::MATERIAL:
          event.m_data[Ptrac::MATERIAL] = tmp;
          break;
        case Ptrac::COLLISION_NUMBER:
          event.m_data[Ptrac::COLLISION_NUMBER] = tmp;
          break;
        case Ptrac::X:
          event.m_data[Ptrac::X] = tmp;
          break;
        case Ptrac::Y:
          event.m_data[Ptrac::Y] = tmp;
          break;
        case Ptrac::Z:
          event.m_data[Ptrac::Z] = tmp;
          break;
        case Ptrac::U:
          event.m_data[Ptrac::U] = tmp;
          break;
        case Ptrac::V:
          event.m_data[Ptrac::V] = tmp;
          break;
        case Ptrac::W:
          event.m_data[Ptrac::W] = tmp;
          break;
        case Ptrac::ENERGY:
          event.m_data[Ptrac::ENERGY] = tmp;
          break;
        case Ptrac::WEIGHT:
          event.m_data[Ptrac::WEIGHT] = tmp;
          break;
        case Ptrac::TIME:
          event.m_data[Ptrac::TIME] = tmp;
          break;
      }
    }

    if( m_format == Ptrac::BIN_PTRAC ) {
      ReadValue(size2);

      if( size1 != size2 ) {
        std::stringstream ss;
        ss << "Failed to read binary PTRAC";
        throw McnpToolsException( ss.str() );
      }
    }

    hist.m_events.push_back(event);
  }

  // make sure to read until end of line if ASCII
  if( m_format == Ptrac::ASC_PTRAC ) {
    std::string chkstr;
    cjsoft::stringops::getline(m_handle, chkstr);
  }

  return hist;
}

std::vector<PtracHistory> Ptrac::ReadHistoriesLegacy(const unsigned int& num) {
  std::vector<PtracHistory> retval;

  for(unsigned int i=0; i<num; i++) {
    m_handle.peek();
    if( !m_handle.eof() ) {
      PtracHistory hist = ReadHistory();
      retval.push_back( hist );
    }
    else {
      break;
    }
  }

  return retval;
}


std::vector<PtracHistory> Ptrac::ReadHistories(const unsigned int& num) {
  return (m_format == Ptrac::HDF5_PTRAC) ? m_hdf5_parser->ReadHistories( num ) :
                                           ReadHistoriesLegacy( num );
}

} // end namespace mcnptools
