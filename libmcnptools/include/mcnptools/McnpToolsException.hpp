#ifndef MCNPTOOLS_MCNPTOOLSEXCEPTION_HPP
#define MCNPTOOLS_MCNPTOOLSEXCEPTION_HPP 1

#include <string>
#include <exception>
#include <iostream>

namespace mcnptools {

class McnpToolsException: public std::exception {
public:
  McnpToolsException(const std::string msg):m_msg(msg) {
#if defined (_MSC_VER)
    std::cerr << "ERROR: " << m_msg << std::endl;
#endif
  };
  ~McnpToolsException() throw() {};

  virtual const char* what() const throw() { return m_msg.c_str(); };

private:
  std::string m_msg;
};

}

#endif
