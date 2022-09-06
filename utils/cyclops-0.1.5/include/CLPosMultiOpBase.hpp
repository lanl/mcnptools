#ifndef __CYCLOPS_CLPOSMULTIOPBASE_HPP__
#define __CYCLOPS_CLPOSMULTIOPBASE_HPP__

#include "CLOpBase.hpp"

namespace cyclops {

class CLPosMultiOpBase : public CLOpBase {
public:
  CLPosMultiOpBase(const std::string& name, const size_t min=1, const size_t max=-1) 
    : CLOpBase(name), m_min_args(min), m_max_args(max) {};

  virtual void Add(const std::string& val)=0;

  virtual size_t Count()=0;

  virtual void Validate()=0;

  size_t MinArgs() const {return m_min_args;};
  size_t MaxArgs() const {return m_max_args;};

private:
  const size_t m_min_args;
  const size_t m_max_args;
};

} // namespace

#endif
