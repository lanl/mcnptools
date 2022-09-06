#ifndef _MCNPTOOLS_PTRACNPS_HPP_
#define _MCNPTOOLS_PTRACNPS_HPP_

#include <map>
#include <stdint.h>

namespace mcnptools {

class PtracNps {
friend class Ptrac;
friend class PtracHistory;
public:
  PtracNps()=default;
  PtracNps(int64_t nps);

  int64_t NPS() const;
  int64_t Cell() const;
  int64_t Surface() const;
  int64_t Tally() const;
  double Value() const;

private:
  int64_t m_nps;
  int64_t m_cell = 0;
  int64_t m_surface = 0;
  int64_t m_tally = 0;
  double  m_value = 0.0;
};

} // end namespace mcnptools

#endif
