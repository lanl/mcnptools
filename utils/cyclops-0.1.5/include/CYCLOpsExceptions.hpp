#ifndef __CYCLOPS_CYCLOPSEXCEPTIONS_HPP__
#define __CYCLOPS_CYCLOPSEXCEPTIONS_HPP__

#include <exception>
#include <string>
#include <iostream>

namespace cyclops {

class CYCLOpsException : public std::exception {
public:
  CYCLOpsException(const std::string& msg) throw() : m_msg(msg) {
#if defined (_MSC_VER)
    std::cerr << "ERROR: " << m_msg << std::endl;
#endif
  };

  virtual const char* what() const throw() {return m_msg.c_str();};

private:
  std::string m_msg;
};

} // namespace

#endif
