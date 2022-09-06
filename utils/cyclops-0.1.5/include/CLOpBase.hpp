#ifndef __CYCLOPS_CLOPBASE_HPP__
#define __CYCLOPS_CLOPBASE_HPP__

#include <string>

namespace cyclops {

class CLOpBase {
public:
  // constructor
  CLOpBase(const std::string& name) 
    : m_name(name), m_meta(name), m_help(""), m_required(false), m_display(true) {};

  // destructor
  virtual ~CLOpBase() {};

  // Sets/Gets
  // -- Name
  std::string Name() const {
    return m_name;
  };

  // -- Help
  std::string Help() const {
    return m_help;
  };
  virtual CLOpBase& Help(const std::string& help) {
    m_help = help;
    return *this;
  };
  
  // -- Meta
  std::string Meta() const {
    return m_meta;
  };
  virtual CLOpBase& Meta(const std::string& meta) {
    m_meta = meta;
    return *this;
  };
  
  // -- Required
  bool Required() const {
    return m_required;
  };
  virtual CLOpBase& Required(const bool& required) {
    m_required = required;
    return *this;
  };
  
  // -- Display
  bool Display() const {
    return m_display;
  };
  virtual CLOpBase& Display(const bool& display) {
    m_display = display;
    return *this;
  };

protected:
  // allow derived to default construct
  CLOpBase() {};

  std::string m_name;     // option name
  std::string m_meta;     // option meta arg
  std::string m_help;     // help string
  bool m_required;        // required option
  bool m_display;         // display in usage
};

} // namespace

#endif
