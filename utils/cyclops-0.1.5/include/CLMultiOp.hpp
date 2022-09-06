#ifndef __CYCLOPS_CLMULTIOP_HPP__
#define __CYCLOPS_CLMULTIOP_HPP__

#include <vector>
#include <cassert>

#include "CLMultiOpBase.hpp"
#include "arg_cast.hpp"

namespace cyclops {

template<typename T>
class CLMultiOp : public CLMultiOpBase {
public:
  CLMultiOp(const std::string& flags, const size_t min=1, const size_t max=-1) 
    : CLMultiOpBase(flags, min, max) {};

  void Add(const std::string& val) {
    m_values.push_back(arg_cast<T>(val));
  }

  size_t Count() {return m_values.size();};

  T Value(size_t i) const {return m_values.at(i);};

  void Validate() {
    assert( m_values.size() >= MinArgs());
    assert( m_values.size() <= MaxArgs());
  };

private:
  std::vector<T> m_values;
};

typedef CLMultiOp<int> CLIntMultiOp;
typedef CLMultiOp<float> CLFloatMultiOp;
typedef CLMultiOp<double> CLDoubleMultiOp;
typedef CLMultiOp<std::string> CLStringMultiOp;

} // namespace

#endif
