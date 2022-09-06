#ifndef SHACL_F5_H5TYPE_COMPOUNDTYPEBULDER_HPP
#define SHACL_F5_H5TYPE_COMPOUNDTYPEBULDER_HPP

#include <memory>
#include <string>
#include <type_traits>

#include "shacl/f5/hdf5_nowarnings.hpp"

#include "shacl/f5/H5Type/Type.hpp"
#include "shacl/f5/H5Type/TypeSupport.hpp"

namespace f5 {

template <typename T> class CompoundTypeBuilder {
  struct TypeInfo {
    Type type;
    std::string name;
    size_t memory_offset;
    size_t file_offset;
    explicit TypeInfo(Type type, std::string name, size_t memory_offset,
                      size_t file_offset)
        : type(std::move(type)), name(std::move(name)),
          memory_offset(memory_offset), file_offset(file_offset) {}
  };

  size_t memory_size;
  size_t current_offset = 0;
  std::vector<TypeInfo> subtypes = {};
  std::shared_ptr<T> class_ptr;

public:
  // This constructor is only to be compiled if T != void.
  // This template allows for it and also guarantees that a T is passed.
  // For C++ API
  template <typename Other,
            typename std::enable_if<
                std::is_same<T, typename std::decay<Other>::type>::value,
                bool>::type = true>
  explicit CompoundTypeBuilder(Other type)
      : memory_size(sizeof(T)),
        class_ptr(std::make_shared<T>(std::move(type))) {}

  explicit CompoundTypeBuilder(size_t size) : memory_size(size) {}

  template <typename class_t, typename member_t>
  CompoundTypeBuilder &addItem(const std::string &name,
                               member_t class_t::*member) {
    auto subtype = getSimpleType<member_t>();
    return addItem(name, member, subtype);
  }

  // For C++ API w/ user-defined types
  template <typename class_t, typename member_t>
  CompoundTypeBuilder &addItem(const std::string &name,
                               member_t class_t::*member, const Type &subtype) {

    // Ensure we're not about to do something horrible
    static_assert(std::is_standard_layout<T>::value,
                  "Object must be standard layout.");
    static_assert(std::is_base_of<class_t, T>::value ||
                      std::is_same<class_t, T>::value,
                  "Object is not a member of class.");

    // Process in-memory portion
    member_t *member_ptr = &(class_ptr.get()->*member);

    const void *class_void_ptr = class_ptr.get();
    const void *member_void_ptr = member_ptr;

    addItem(name, class_void_ptr, member_void_ptr, subtype);

    return *this;
  }

  // Raw Pointer Form
  void addItem(std::string name, const void *class_void_ptr,
               const void *member_void_ptr, const Type &subtype) {
    // Process in-memory portion
    // TODO(cjosey) : replace char with std::byte once c++17 is better supported

    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    auto member_loc = reinterpret_cast<const char *>(member_void_ptr);
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    auto class_loc = reinterpret_cast<const char *>(class_void_ptr);
    size_t mem_offset = member_loc - class_loc;

    // Process on-file portion
    size_t file_size = subtype.file_type.getSize();
    size_t file_offset = current_offset;

    subtypes.push_back(
        TypeInfo(subtype, std::move(name), mem_offset, file_offset));

    // Update file offset
    current_offset += file_size;
  }

  Type build() const {
    // Construct the two types
    auto memory_type = H5Tcreate(H5T_COMPOUND, memory_size);
    auto file_type = H5Tcreate(H5T_COMPOUND, current_offset);
    for (auto const &type : subtypes) {
      H5Tinsert(memory_type, type.name.c_str(), type.memory_offset,
                type.type.memory_type.getID());
      H5Tinsert(file_type, type.name.c_str(), type.file_offset,
                type.type.file_type.getID());
    }
    BasicType mtype(memory_type, true);
    BasicType ftype(file_type, true);

    return Type(mtype, ftype);
  }
};

template <typename T> auto makeCompoundTypeBuilder(T &&type) {
  return CompoundTypeBuilder<typename std::decay<T>::type>{
      std::forward<T>(type)};
}

using FortranTypeBuilder = CompoundTypeBuilder<void>;

} // namespace f5

#endif
