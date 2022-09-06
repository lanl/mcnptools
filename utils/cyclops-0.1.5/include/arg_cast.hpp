#ifndef __CYCLOPTS_ARG_CAST_HPP__
#define __CYCLOPTS_ARG_CAST_HPP__

#include <sstream>
#include <cstdlib>

#include "CYCLOpsExceptions.hpp"

namespace cyclops {

template<typename T>
T arg_cast(const std::string& s) {
  T retval;
  std::stringstream ss(s);
  ss >> retval;
  // if( !ss.fail() )
  //   throw CYCLOpsException("failed to perform argument cast for \'" + s + "\'"); 
  return retval;
};

template<>
int arg_cast(const std::string& s) {
  int retval;
  try {
    retval = std::stoi(s);
  }
  catch(...) {
    throw CYCLOpsException("error converting \'" + s + "\' to integer");
  }

  return retval;
};

template<>
double arg_cast(const std::string& s) {
  double retval;
  try {
    retval = std::stof(s);
  }
  catch(...) {
    throw CYCLOpsException("error converting \'" + s + "\' to double");
  }

  return retval;
};

template<>
std::string arg_cast<std::string>(const std::string& s) {
  return s;
};

} // namespace

#endif
