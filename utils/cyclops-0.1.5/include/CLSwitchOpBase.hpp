#ifndef __CYCLOPTS_CLSWITCHOPBASE_HPP__
#define __CYCLOPTS_CLSWITCHOPBASE_HPP__

#include "CLFlagOpBase.hpp"

namespace cyclops {

class CLSwitchOpBase : public CLFlagOpBase {
public:
  CLSwitchOpBase(const std::string& flags) : CLFlagOpBase(flags) {};

  virtual ~CLSwitchOpBase() {};

  virtual void Switch()=0;

protected:
};

} // namespace

#endif
