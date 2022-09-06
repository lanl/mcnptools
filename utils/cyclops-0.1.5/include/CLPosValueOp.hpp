#ifndef __CYCLOPS_CLPOSVALUEOP_HPP__
#define __CYCLOPS_CLPOSVALUEOP_HPP__

#include "CLPosValueOpBase.hpp"
#include "arg_cast.hpp"

namespace cyclops {

template<typename T>
class CLPosValueOp : public CLPosValueOpBase {
public:
  CLPosValueOp(const std::string& name, const T def=T()) : CLPosValueOpBase(name), m_value(def) {};

  void Set(const std::string& val) {
    m_value = arg_cast<T>(val);
  }

  T Value() const {return m_value;};

private:
  T m_value;
};

typedef CLPosValueOp<int> CLIntPosValueOp;
typedef CLPosValueOp<float> CLFloatPosValueOp;
typedef CLPosValueOp<double> CLDoublePosValueOp;
typedef CLPosValueOp<std::string> CLStringPosValueOp;

} // namespace

#endif
