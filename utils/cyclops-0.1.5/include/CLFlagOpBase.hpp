#ifndef __CYCLOPS_CLFLAGOPBASE_HPP__
#define __CYCLOPS_CLFLAGOPBASE_HPP__

#include <string>
#include <set>

#include "CYCLOpsExceptions.hpp"
#include "CLOpBase.hpp"

namespace cyclops {

class CLFlagOpBase : public CLOpBase {
public:
  // constructor
  CLFlagOpBase(const std::string& flags) {
    ParseFlags(flags);
    m_name = m_long_flag.substr(2,m_long_flag.size()-2);
    m_meta = m_name;
    m_required = false;
    m_display = true;
  };

  virtual ~CLFlagOpBase() {};

  bool HasShortFlag() const {return m_has_short;};

  std::string LongFlag() const {return m_long_flag;};
  std::string ShortFlag() const {
    std::string retval = m_long_flag; // should this be?
    if( HasShortFlag() ) {
      retval = m_short_flag;
    }
    return retval;
  };

protected:
  // allow derived to default construct
  CLFlagOpBase() {};

  std::string m_long_flag;  // long flag
  std::string m_short_flag; // short flag
  bool m_has_short;

  // no short flags with these characters
  static constexpr const char* m_short_forbidden = "-\"\',.<>:;{}[]|\\_+/=~`";

  void ParseFlags(const std::string& flags) {
    m_has_short = false;

    size_t ddpos = flags.find("--");

    if( ddpos != 0 )
      throw CYCLOpsException("could not parse long option out of \'" + flags + "\'");

    size_t cpos = flags.find(",");

    if( cpos == std::string::npos ) {
      m_long_flag = flags;
    }
    else {
      m_has_short = true;
      m_long_flag = flags.substr(0,cpos);

      if( flags.find(",",cpos+1) != std::string::npos )
        throw CYCLOpsException("found too many \',\'s parsing flags variable \'" + flags + "\'");

      if( flags.size() - (cpos+1) != static_cast<size_t>(2) )
        throw CYCLOpsException("could not parse short option out of \'" + flags + "\'");

      m_short_flag = flags.substr(cpos+1,2);

      if( m_short_flag.find("-") != 0 )
        throw CYCLOpsException("could not parse short option out of \'" + flags + "\'");

      if( std::string(m_short_forbidden).find(m_short_flag[1]) != std::string::npos )
        throw CYCLOpsException("short option character \'" + m_short_flag + "\' is forbidden");
    }
  };
};

} // namespace

#endif
