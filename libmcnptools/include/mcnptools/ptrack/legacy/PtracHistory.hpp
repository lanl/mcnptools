#ifndef _MCNPTOOLS_PTRACHISTORY_HPP_
#define _MCNPTOOLS_PTRACHISTORY_HPP_

#include <vector>

#include "mcnptools/ptrack/legacy/PtracNps.hpp"
#include "mcnptools/ptrack/legacy/PtracEvent.hpp"

namespace mcnptools {

class PtracHistory {
friend class Ptrac;

public:
  PtracHistory()=default;
  PtracHistory(PtracNps, std::vector<PtracEvent>&&);

  PtracNps GetNPS() const { return m_nps; }
  unsigned int GetNumEvents() const { return m_events.size(); } 
  PtracEvent GetEvent(const unsigned int& i) const { return m_events.at(i); } 

private:
  PtracNps m_nps;
  std::vector<PtracEvent> m_events;
};

}

#endif
