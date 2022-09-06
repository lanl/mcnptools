#ifndef __CYCLOPTS_CLSWITCHOP_HPP__
#define __CYCLOPTS_CLSWITCHOP_HPP__

#include "CLSwitchOpBase.hpp"

namespace cyclops {

template<typename T, T start, T toggle>
class CLSwitchOp : public CLSwitchOpBase {
public:
  CLSwitchOp(const std::string& flags) : CLSwitchOpBase(flags), m_value(start) {};

  void Switch() {m_value = toggle;};

  T Value() const {return m_value;};

protected:
  T m_value;
};

typedef CLSwitchOp<bool, false, true> CLTrueSwitchOp;
typedef CLSwitchOp<bool, true, false> CLFalseSwitchOp;

} // namespace

#endif
