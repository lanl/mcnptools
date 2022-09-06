#include "mcnptools/ptrack/legacy/PtracHistory.hpp"

namespace mcnptools {

PtracHistory::PtracHistory(PtracNps nps, std::vector<PtracEvent>&& events):
  m_nps(nps),
  m_events( std::move(events) )
{}

}
