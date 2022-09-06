#ifndef __CYCLOPS_CLMULTIOPBASE_HPP__
#define __CYCLOPS_CLMULTIOPBASE_HPP__

#include "CLFlagOpBase.hpp"

namespace cyclops {

class CLMultiOpBase : public CLFlagOpBase {
public:
  CLMultiOpBase(const std::string& flags, const size_t min=1, const size_t max=-1) 
    : CLFlagOpBase(flags), m_min_args(min), m_max_args(max) {};

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
