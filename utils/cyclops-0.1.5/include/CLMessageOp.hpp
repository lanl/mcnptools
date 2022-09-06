#ifndef __CYCLOPS_CLMESSAGEOP_HPP__
#define __CYCLOPS_CLMESSAGEOP_HPP__

#include <iostream>

#include "CLFlagOpBase.hpp"

namespace cyclops {

class CLMessageOp : public CLFlagOpBase {
public:
  CLMessageOp(const std::string& flags, const std::string& message, const bool exit=true, std::ostream& out=std::cerr)
    : CLFlagOpBase(flags),
      m_message(message),
      m_exit(exit),
      m_out(out)
  {};

  std::string GetMessage() const { return m_message; };

  bool GetExit() const { return m_exit; };

  std::ostream& GetOstream() const { return m_out; };

private:
  std::string m_message;
  bool m_exit;
  std::ostream& m_out;
};

} // namespace

#endif
