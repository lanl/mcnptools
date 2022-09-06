#ifndef __CYCLOPS_CLVALUEOP_HPP__
#define __CYCLOPS_CLVALUEOP_HPP__

#include "CLValueOpBase.hpp"
#include "arg_cast.hpp"

namespace cyclops {

template<typename T>
class CLValueOp : public CLValueOpBase {
public:
  CLValueOp(const std::string& flags, const T def=T()) : CLValueOpBase(flags), m_value(def) {};

  void Set(const std::string& val) {
    m_value = arg_cast<T>(val);
  }

  T Value() const {return m_value;};

private:
  T m_value;
};

typedef CLValueOp<int> CLIntValueOp;
typedef CLValueOp<float> CLFloatValueOp;
typedef CLValueOp<double> CLDoubleValueOp;
typedef CLValueOp<std::string> CLStringValueOp;

} // namespace

#endif
