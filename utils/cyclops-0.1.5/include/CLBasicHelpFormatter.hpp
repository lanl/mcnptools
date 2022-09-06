#ifndef __CYCLOPS_CLBASICHELPFORMATTER_HPP__
#define __CYCLOPS_CLBASICHELPFORMATTER_HPP__

#include <sstream>
#include <iterator>

#include "CLHelpFormatterBase.hpp"

namespace cyclops {

template<size_t W=80, size_t O=20>
class CLBasicHelpFormatter : public CLHelpFormatterBase {
public:
  CLBasicHelpFormatter(const std::string& prog, const std::string desc="", const std::string epi="") 
    : CLHelpFormatterBase(prog, desc, epi) {
    m_usage = "USAGE: " + m_prog + " ";
    m_usage_space = std::string(7,' ');
    m_options = "OPTIONS:\n\n";
  };

  void AddFlaggedOp(const CLFlagOpBase& fop, size_t min=1, size_t max=1) {
    // handle usage description
    if( fop.Display() ) {
      std::string bmark = m_o_begin;
      std::string emark = m_o_end;;
      if( fop.Required() ) {
        bmark = "<";
        emark = ">";
      }

      std::string ui;
      ui += bmark + fop.LongFlag();
      if( min > 0 ) {
        ui += " " + fop.Meta();
      }
      if( min > 0 && max > min ) {
        ui += " [" + fop.Meta() + " ... ]";
      }
      ui += emark;
      AddToUsage(ui);
    }

    // handle options description
    std::string help = fop.Help();
    std::string lf = fop.LongFlag();
    std::string sf = fop.ShortFlag();

    if( help.size() > 0 ) {
      std::string opt;
      opt += fop.LongFlag();
      if( fop.LongFlag() != fop.ShortFlag() ) {
        opt += ", " + fop.ShortFlag();
      }
      if( opt.size() < O-2 ) {
        opt += std::string(O-opt.size()-2,' ') + ": ";
      }
      else {
        opt += ": ";
      }
      m_options += opt;
      AddToOptions(fop.Help());
      m_options += "\n\n";
    }
  };

  void AddPositionalOp(const CLOpBase& op, size_t min=1, size_t max=1) {
    // handle usage description
    if( op.Display() ) {
      std::string bmark = m_o_begin;
      std::string emark = m_o_end;
      if( op.Required() ) {
        bmark = m_r_begin;
        emark = m_r_end;
      }

      std::string ui;
      ui += bmark + op.Meta();
      if( max > min ) {
        ui += " [" + op.Meta() + " ... ]";
      }
      ui += emark;
      AddToUsage(ui);
    }

    // handle options description
    std::string help = op.Help();

    if( help.size() > 0 ) {
      std::string opt;
      opt += op.Meta();
      if( opt.size() < O-2 ) {
        opt += std::string(O-opt.size()-2,' ') + ": ";
      }
      else {
        opt += ": ";
      }
      m_options += opt;
      AddToOptions(op.Help());
      m_options += "\n\n";
    }
  };

  void operator()(std::ostream& os) {
    os << m_usage << std::endl << std::endl;
    if( m_description.size() > 0 ) {
      os << "DESCRIPTION:" << std::endl << std::endl;
      os << Format(m_description) << std::endl << std::endl;
    }
    os << m_options;
    if( m_epilog.size() > 0 ) {
      os << Format(m_epilog) << std::endl << std::endl;
    }
  };

private:
  std::string m_usage;
  std::string m_usage_space;
  std::string m_options;

  const std::string m_o_begin = "[";
  const std::string m_o_end = "]";
  const std::string m_r_begin = "<";
  const std::string m_r_end = ">";

  std::string Format(const std::string& s) {
    std::string retval;

    std::vector<std::string> words = SplitString(s);
    for(auto it=words.begin(); it!=words.end(); ++it) {
      std::string word = *it;

      if( word == "$0" ) word = m_prog;

      size_t last_nl = retval.rfind("\n");
      std::string subs;
      if( last_nl == std::string::npos ) {
        subs = retval;
      }
      else {
        subs = retval.substr(last_nl);
      }
      
      if( subs.size() + it->size() + 1 > W ) {
        retval += "\n" + word + " ";
      }
      else {
        retval += word + " ";
      }
    }

    return retval;
  };

  void AddToUsage(const std::string& ui) {
    size_t last_nl = m_usage.rfind("\n");
    std::string subs;
    if( last_nl == std::string::npos ) {
      subs = m_usage;
    }
    else {
      subs = m_usage.substr(last_nl);
    }
    
    if( subs.size() + ui.size() > W ) {
      m_usage += "\n" + m_usage_space + ui + " ";
    }
    else {
      m_usage += ui + " ";
    }
  };

  void AddToOptions(const std::string& ui) {
    std::vector<std::string> words = SplitString(ui);
    for(auto it=words.begin(); it!=words.end(); ++it) {
      size_t last_nl = m_options.rfind("\n");
      std::string subs;
      if( last_nl == std::string::npos ) {
        subs = m_options;
      }
      else {
        subs = m_options.substr(last_nl);
      }
      
      if( subs.size() + it->size() + 1 > W ) {
        m_options += "\n" + std::string(O,' ') + *it + " ";
      }
      else {
        m_options += *it + " ";
      }
    }
  };

  std::vector<std::string> SplitString(const std::string& s) {
    std::istringstream buf(s);
    std::istream_iterator<std::string> beg(buf), end;
    std::vector<std::string> retval(beg, end);
    return retval;
  };
};


} // namespace

#endif
