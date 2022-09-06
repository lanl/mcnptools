#include "mcnptools/ptrack/legacy/PtracNps.hpp"

namespace mcnptools {

PtracNps::PtracNps(int64_t nps):
  m_nps( nps )
{}

int64_t PtracNps::NPS() const {
  return m_nps;
}

int64_t PtracNps::Cell() const {
  return m_cell;
}

int64_t PtracNps::Surface() const {
  return m_surface;
}

int64_t PtracNps::Tally() const {
  return m_tally;
}

double PtracNps::Value() const {
  return m_value;
}

} // end namespace mcnptools
