#ifndef _MCNPTOOLS_PTRACK_HPP_
#define _MCNPTOOLS_PTRACK_HPP_

#include <unordered_map>
#include <map>
#include <functional>
#include <type_traits>
#include <cmath>
#include <stdexcept>
#include <string>
#include <algorithm>

#include "shacl/f5.hpp"

#include "mcnptools/Constants.hpp"
#include "mcnptools/ptrack/PtracEnums.hpp"
#include "mcnptools/ptrack/legacy/PtracEvent.hpp"
#include "mcnptools/ptrack/legacy/PtracNps.hpp"
#include "mcnptools/ptrack/legacy/PtracHistory.hpp"

namespace mcnptools
{
  #include "mcnptools/ptrack/hdf5/Events.hpp"
  namespace detail {
    #include "mcnptools/ptrack/hdf5/detail.hpp"
  }
  #include "mcnptools/ptrack/hdf5/H5Compounds.hpp"
  #include "mcnptools/ptrack/hdf5/DatasetBuffer.hpp"
  #include "mcnptools/ptrack/hdf5/converter.hpp"
  #include "mcnptools/ptrack/hdf5/EventParsers.hpp"
  #include "mcnptools/ptrack/hdf5/RecordLog.hpp"
  #include "mcnptools/ptrack/hdf5/HistoryParser.hpp"
}

#endif
