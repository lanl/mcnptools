#include "mcnptools/ptrack/legacy/PtracEvent.hpp"
#include "mcnptools/ptrack/PtracEnums.hpp"

namespace mcnptools {

PtracEvent::PtracEvent(int type, const std::map<int,double>& data):
  m_type(type),
  m_data(data)
{}

PtracEvent::PtracEvent(int type, std::map<int,double>&& data):
  m_type(type),
  m_data( std::move(data) )
{}

int PtracEvent::Type() const {
  return m_type;
};

int PtracEvent::BankType() const {
  if (m_bnktype > 0 ) {
    return m_bnktype;
  }
  //Look in the data map for the bank type, because HDF5 stores it there
  else if ( m_data.find( PtracEnums::BANK_TYPE ) != m_data.end() ) {
    return static_cast<int>( m_data.at(PtracEnums::BANK_TYPE) );
  }
  else
  {
    throw std::runtime_error( "Tried to access bank type for non-bank event");
  }
}

bool PtracEvent::Has(const int& d) const {
  bool retval = false;
  if( m_data.find( d ) != m_data.end() )
    retval = true;

  return retval;
}

double PtracEvent::Get(const int& d) const {
  return m_data.at(d);
}

} // end namespace mcnptools
