#ifndef _MCNPTOOLS_PTRACEVENT_HPP_
#define _MCNPTOOLS_PTRACEVENT_HPP_

#include <stdexcept>
#include <map>

namespace mcnptools {

class PtracEvent {
friend class Ptrac;
friend class PtracHistory;
public:
  PtracEvent(int type, const std::map<int,double>& data);
  PtracEvent(int type, std::map<int,double>&& data);

  int Type() const;
  int BankType() const;
  bool Has(const int& d) const;
  double Get(const int& d) const;

private:
  PtracEvent()=default;
  int m_type;
  int m_bnktype = -1;
  std::map<int,double> m_data;
};

} // end namespace mcnptools

#endif
