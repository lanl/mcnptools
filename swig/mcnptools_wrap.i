#if defined(SWIGPYTHON)
%module "mcnptools_wrap"
#elif defined(SWIGPERL)
%module "McnpTools"
#endif

%{
#include "mcnptools/McnpTools.hpp"
%}

%include "std_string.i"
%include "std_vector.i"
%include "exception.i"

%template(vector_int) std::vector<int>;
%template(vector_double) std::vector<double>;
%template(vector_string) std::vector<std::string>;
%template(vector_PtracHistory) std::vector<mcnptools::PtracHistory>;

%rename(mergeMctals) mcnptools::mergeMctals(const std::string& name, const std::vector<std::string>& mctals, const unsigned int verbosity);
%rename(mergeMeshtals) mcnptools::mergeMctals(const std::string& name, const std::vector<std::string>& meshtals, const unsigned int verbosity);
%rename(l3dinfo) mcnptools::l3dinfo(const std::vector<std::string>& files, const bool full=false);

// TYPEMAP for int64_t
#if defined(SWIGPYTHON)
%typemap(out) int64_t {
  $result = PyInt_FromLong($1);
}
#elif defined(SWIGPERL)
%typemap(out) int64_t {
  $result = sv_newmortal();
  sv_setiv($result,(IV) $1);
  argvi++;
}
#endif

%exception{
  try {
    $action
  }
  catch (mcnptools::McnpToolsException) {
    SWIG_exception_fail(SWIG_UnknownError,"McnpToolsException");
  }
  SWIG_CATCH_STDEXCEPT // catch std::exception
  catch (...) {
    SWIG_exception_fail(SWIG_UnknownError, "Unknown exception");
  }
} 

// order matters here!!!
%include "../libmcnptools/include/mcnptools/Lnk3Dnt.hpp"

%include "../libmcnptools/include/mcnptools/MctalKcode.hpp"
%include "../libmcnptools/include/mcnptools/MctalTally.hpp"
%include "../libmcnptools/include/mcnptools/Mctal.hpp"

%include "../libmcnptools/include/mcnptools/MeshtalTally.hpp"
%include "../libmcnptools/include/mcnptools/Meshtal.hpp"

%include "../libmcnptools/include/mcnptools/ptrack/PtracEnums.hpp"
%include "../libmcnptools/include/mcnptools/ptrack/legacy/PtracNps.hpp"
%include "../libmcnptools/include/mcnptools/ptrack/legacy/PtracEvent.hpp"
%include "../libmcnptools/include/mcnptools/ptrack/legacy/PtracHistory.hpp"
%include "../libmcnptools/include/mcnptools/Ptrac.hpp"

%include "../libmcnptools/include/mcnptools/XSDir.hpp"

%include "../libmcnptools/include/mcnptools/mergeMctals.hpp"
%include "../libmcnptools/include/mcnptools/mergeMeshtals.hpp"

