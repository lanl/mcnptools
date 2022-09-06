#ifndef __CYCLOPS_CLPOSVALUEOPBASE_HPP__
#define __CYCLOPS_CLPOSVALUEOPBASE_HPP__

#include "CLOpBase.hpp"

namespace cyclops {

class CLPosValueOpBase : public CLOpBase {
public:
  CLPosValueOpBase(const std::string& name) : CLOpBase(name) {};

  virtual void Set(const std::string& val)=0;

private:

};

} // namespace

#endif
