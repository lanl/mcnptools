#ifndef SHACL_F5_H5TYPE_TYPE_HPP
#define SHACL_F5_H5TYPE_TYPE_HPP

#include "shacl/f5/hdf5_nowarnings.hpp"

#include "shacl/f5/H5Holders.hpp"

namespace f5 {

class BasicType : public Handle<H5Tclose> {
public:
  // Default to an invalid type
  explicit BasicType() : Handle(-1, false) {}
  explicit BasicType(hid_t id, bool manage) : Handle(id, manage) {}

  size_t getSize() const { return H5Tget_size(this->getID()); }
};

class Type {
public:
  BasicType memory_type;
  BasicType file_type;
  // Default to an invalid type pair
  explicit Type() = default;
  explicit Type(BasicType memory_type, BasicType file_type)
      : memory_type(std::move(memory_type)), file_type(std::move(file_type)) {}
};

} // namespace f5

#endif
