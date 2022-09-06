#ifndef __CYCLOPS_CLVALUEOPBASE_HPP__
#define __CYCLOPS_CLVALUEOPBASE_HPP__

#include "CLFlagOpBase.hpp"

namespace cyclops {

class CLValueOpBase : public CLFlagOpBase {
public:
  CLValueOpBase(const std::string& flags) : CLFlagOpBase(flags) {};

  virtual void Set(const std::string& val)=0;

private:

};

} // namespace

#endif
