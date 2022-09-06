#ifndef __CYCLOPS_CLHELPFORMATTERBASE_HPP__
#define __CYCLOPS_CLHELPFORMATTERBASE_HPP__

#include <ostream>

namespace cyclops {

class CLHelpFormatterBase {
public:
  CLHelpFormatterBase(const std::string& prog, const std::string desc="", const std::string epi="")  
    : m_prog(prog), m_description(desc), m_epilog(epi) {};
  virtual ~CLHelpFormatterBase() {};

  virtual void AddFlaggedOp(const CLFlagOpBase& fop, size_t min=1, size_t max=1)=0;
  virtual void AddPositionalOp(const CLOpBase& fop, size_t min=1, size_t max=1)=0;

  virtual void operator()(std::ostream& os)=0;

  std::string Description() const {
    return m_description;
  };
  CLHelpFormatterBase& Description(const std::string& desc) {
    m_description = desc;
    return *this;
  };

  std::string Epilog() const {
    return m_epilog;
  };
  CLHelpFormatterBase& Epilog(const std::string& epi) {
    m_epilog = epi;
    return *this;
  };

protected:
  std::string m_prog;
  std::string m_description;
  std::string m_epilog;
};

} // namespace

#endif
