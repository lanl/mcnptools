#ifndef __CYCLOPS_CLPOSMULTIOP_HPP__
#define __CYCLOPS_CLPOSMULTIOP_HPP__

#include <vector>
#include <cassert>

#include "CLPosMultiOpBase.hpp"
#include "arg_cast.hpp"

namespace cyclops {

template<typename T>
class CLPosMultiOp : public CLPosMultiOpBase {
public:
  CLPosMultiOp(const std::string& name, const size_t min=1, const size_t max=-1) 
    : CLPosMultiOpBase(name, min, max) {};

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

typedef CLPosMultiOp<int> CLIntPosMultiOp;
typedef CLPosMultiOp<float> CLFloatPosMultiOp;
typedef CLPosMultiOp<double> CLDoublePosMultiOp;
typedef CLPosMultiOp<std::string> CLStringPosMultiOp;

} // namespace

#endif
