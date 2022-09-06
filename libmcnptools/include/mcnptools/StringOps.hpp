#ifndef __CJSOFT_STRINGOPS_HPP__
#define __CJSOFT_STRINGOPS_HPP__

#include <string>

#ifndef __MCNPTOOLS_BOOST_REGEX__

  #include <regex>

  namespace reg = std;

#else

  #include <boost/regex.hpp>

  namespace reg = boost;

#endif

namespace cjsoft {

namespace stringops {

class StringLTrim {
  public:
    StringLTrim(std::string xchars=" \n\t\r") : m_xchars(xchars) {};
    std::string operator()(const std::string& str) const {
      std::string retval(str);
      retval.erase(0,retval.find_first_not_of(m_xchars));
      return retval;
    }
  private:
    const std::string m_xchars;
};

inline void ltrim(std::string& str) {
  str = StringLTrim()(str);
}

class StringRTrim {
  public:
    StringRTrim(std::string xchars=" \n\t\r") : m_xchars(xchars) {};
    std::string operator()(const std::string& str) const {
      std::string retval(str);
      retval.erase(str.find_last_not_of(m_xchars)+1);
      return retval;
    };
  private:
    const std::string m_xchars;
};

inline void rtrim(std::string& str) {
  str = StringRTrim()(str);
}

class StringTrim {
  public:
    StringTrim(std::string xchars=" \n\t\r") : m_xchars(xchars) {};
    std::string operator()(const std::string& str) const {
      std::string retval(str);
      retval = StringLTrim(m_xchars)(str);
      retval = StringRTrim(m_xchars)(retval);
      return retval;
    }
  private:
    const std::string m_xchars;
};

inline void trim(std::string& str) {
  str = StringTrim()(str);
}

template<class ContainerT, class FunctorT>
void tokenize(const std::string& str,
              ContainerT& tokens,
              const std::string& delimiters = " ",
              const FunctorT& strop = FunctorT())
{
    // Skip delimiters at beginning.
    std::string::size_type lastPos = str.find_first_not_of(delimiters, 0);
    // Find first "non-delimiter".
    std::string::size_type pos     = str.find_first_of(delimiters, lastPos);

    while (std::string::npos != pos || std::string::npos != lastPos)
    {
        // Found a token, add it to the vector.
        tokens.push_back(strop(str.substr(lastPos, pos - lastPos)));
        // Skip delimiters.  Note the "not_of"
        lastPos = str.find_first_not_of(delimiters, pos);
        // Find next "non-delimiter"
        pos = str.find_first_of(delimiters, lastPos);
    }
}

template<class ContainerT>
void tokenize(const std::string& str,
              ContainerT& tokens,
              const std::string& delimiters = " ")
{
    // Skip delimiters at beginning.
    std::string::size_type lastPos = str.find_first_not_of(delimiters, 0);
    // Find first "non-delimiter".
    std::string::size_type pos     = str.find_first_of(delimiters, lastPos);

    while (std::string::npos != pos || std::string::npos != lastPos)
    {
        // Found a token, add it to the vector.
        tokens.push_back(str.substr(lastPos, pos - lastPos));
        // Skip delimiters.  Note the "not_of"
        lastPos = str.find_first_not_of(delimiters, pos);
        // Find next "non-delimiter"
        pos = str.find_first_of(delimiters, lastPos);
    }
}

inline double fortran3digitfixup( std::string s ) {
  reg::regex noematch ("(\\d)(\\+|\\-)");
  s = reg::regex_replace(s, noematch, "\\1e\\2");
  return std::stod( s );
}

inline std::istream& getline(std::istream& is, std::string& str) {
    std::getline(is, str, '\n');

    while( str.size() != 0 && str[ str.size()-1 ] == '\r' )
        str = str.substr(0, str.size()-1);

    return is;
}

} // namespace stringops

} // namespace cjsoft

#endif
