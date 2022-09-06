#ifndef SHACL_F5_H5TYPE_TYPESUPPORT_HPP
#define SHACL_F5_H5TYPE_TYPESUPPORT_HPP

#include <cstdint>
#include <cstring>
#include <stdexcept>

#include "shacl/f5/hdf5_nowarnings.hpp"

#include "shacl/f5/H5Type/Type.hpp"

namespace f5 {

namespace detail {
// Special functions
template <typename T>
inline auto getIntBoolType(size_t size, T *mem_true, T *mem_false) {
  // Create memory type
  auto type = H5Tcreate(H5T_ENUM, size);

  if (type < 0) {
    throw std::runtime_error("Unable to create HDF5 type.");
  }

  H5Tenum_insert(type, "FALSE", mem_false);
  H5Tenum_insert(type, "TRUE", mem_true);

  BasicType mtype(type, true);

  // Create file type (INT32 [0, 1] "bool")
  std::int32_t truthy = 1;
  std::int32_t falsy = 0;
  type = H5Tcreate(H5T_ENUM, sizeof(std::int32_t));

  if (type < 0) {
    throw std::runtime_error("Unable to create HDF5 type.");
  }

  H5Tenum_insert(type, "FALSE", &falsy);
  H5Tenum_insert(type, "TRUE", &truthy);

  BasicType ftype(type, true);

  return Type(mtype, ftype);
}

inline auto getFortranStringType(std::int64_t str_size) {
  // Create memory type (fortran space-padded string)
  auto type = H5Tcopy(H5T_FORTRAN_S1);

  if (type < 0) {
    throw std::runtime_error("Unable to create HDF5 type.");
  }

  H5Tset_size(type, str_size);

  BasicType mtype(type, true);

  // Create file type (copy of above - using H5T_C_S1 causes mem issues)
  type = H5Tcopy(H5T_FORTRAN_S1);

  if (type < 0) {
    throw std::runtime_error("Unable to create HDF5 type.");
  }

  H5Tset_size(type, str_size);

  BasicType ftype(type, true);

  return Type(mtype, ftype);
}

// This class is a compiler trick to delay the instantiation of
// static assert until after one of the other tempaltes is a better match
template <typename T>
struct UnknownTypeError {
  static_assert(sizeof(T) < 0,
                "the type of this object is unknown to F5 library");
};

template <typename T, std::enable_if_t<!std::is_signed<T>::value, bool> = true>
constexpr auto getRawInt() {
  // Assumes will be run on 64 bit machine. Sizes picked to prevent data loss.
  auto size = sizeof(T) * CHAR_BIT;
  if (size <= 8) {
    return H5T_STD_U8LE;
  } else if (size <= 16) {
    return H5T_STD_U16LE;
  } else if (size <= 32) {
    return H5T_STD_U32LE;
  }
  return H5T_STD_U64LE;
}

template <typename T, std::enable_if_t<std::is_signed<T>::value, bool> = true>
constexpr auto getRawInt() {
  // Assumes will be run on 64 bit machine. Sizes picked to prevent data loss.
  auto size = sizeof(T) * CHAR_BIT;
  if (size <= 8) {
    return H5T_STD_I8LE;
  } else if (size <= 16) {
    return H5T_STD_I16LE;
  } else if (size <= 32) {
    return H5T_STD_I32LE;
  }
  return H5T_STD_I64LE;
}

inline constexpr char getStringPadChar(H5T_str_t type) {
  if (type == H5T_STR_NULLTERM) {
    return '\0';
  } else if (type == H5T_STR_NULLPAD) {
    return '\0';
  } else if (type == H5T_STR_SPACEPAD) {
    return ' ';
  } else {
    throw std::runtime_error("Unknown string termination type");
  }
}

} // namespace detail

template <size_t Size, H5T_str_t pad_type = H5T_STR_NULLTERM>
class FixedLengthString {
private:
  // It is standard guaranteed that chars_ will be the starting
  // address of the class, and that std::array is an aggregate class
  // so memory can be directly read into array from HDF5 dataset
  std::array<char, Size> chars_{}; // Extra memory for null terminator
  static constexpr char pad_char_() {
    return detail::getStringPadChar(pad_type);
  }
  static constexpr char null_char_() {
    return char{0};
  }
  static_assert(Size > 0, "must write a non-empty fixed length string");

public:
  FixedLengthString() {
    std::fill(chars_.begin(), chars_.end(), pad_char_());
  }

  FixedLengthString(const char *s) {
    auto str_size = std::min(std::strlen(s), Size);
    auto last = std::copy(s, s + str_size, chars_.begin());
    std::fill(last, chars_.end(), pad_char_());
  }

  FixedLengthString(const std::string &s)
      : FixedLengthString(s.c_str()) // Delegating Constructor
  {}

  auto to_string() const {
    // Find where right padding begins by searching from
    // the right of the string for first non-pad character
    constexpr auto pad_char = pad_char_();
    const auto not_pad_char = [=](const char c) { return c != pad_char; };
    const auto rend =
        std::find_if(chars_.rbegin(), chars_.rend(), not_pad_char);
    const auto fend = rend.base(); // Convert to forward iterator

    return std::string(chars_.begin(), fend);
  }

  static constexpr size_t size() {
    return Size;
  }
};

inline auto getFixedStringType(size_t N,
                               H5T_str_t pad_type = H5T_STR_NULLTERM) {
  // Create memory type (null-terminated C string)
  auto type = H5Tcopy(H5T_C_S1);
  if (type < 0) {
    throw std::runtime_error("Unable to create HDF5 c string type.");
  }

  H5Tset_strpad(type, pad_type);
  H5Tset_size(type, N);

  BasicType mtype(type, true);
  auto ftype = mtype;

  return Type(mtype, ftype);
}

template <typename T>
struct TypeTag {};

// Default instantiation will always static assert an error
template <typename T>
auto getSimpleTypeSpecialImpl(TypeTag<T>) {
  detail::UnknownTypeError<T>();
}

template <size_t N, H5T_str_t pad_type>
auto getSimpleTypeSpecialImpl(TypeTag<FixedLengthString<N, pad_type>>) {
  return getFixedStringType(N, pad_type);
}

template <typename T>
auto getSimpleType() {
  return getSimpleTypeSpecialImpl(TypeTag<T>{});
}

template <>
inline auto getSimpleType<std::string>() {

  // Create memory type (C nullterm-padded string)
  auto type = H5Tcopy(H5T_C_S1);

  if (type < 0) {
    throw std::runtime_error("Unable to create HDF5 c string type.");
  }

  H5Tset_size(type, H5T_VARIABLE);

  BasicType mtype(type, true);
  auto ftype = mtype;

  return Type(mtype, ftype);
}

// Integer types

template <>
inline auto getSimpleType<unsigned char>() {
  BasicType mtype(H5T_NATIVE_UCHAR, false);
  BasicType ftype(detail::getRawInt<unsigned char>(), false);

  return Type(mtype, ftype);
}

template <>
inline auto getSimpleType<signed char>() {
  BasicType mtype(H5T_NATIVE_SCHAR, false);
  BasicType ftype(detail::getRawInt<signed char>(), false);

  return Type(mtype, ftype);
}

template <>
inline auto getSimpleType<char>() {
  BasicType mtype(H5T_NATIVE_CHAR, false);
  BasicType ftype(detail::getRawInt<char>(), false);

  return Type(mtype, ftype);
}

template <>
inline auto getSimpleType<unsigned short>() {
  BasicType mtype(H5T_NATIVE_USHORT, false);
  BasicType ftype(detail::getRawInt<unsigned short>(), false);

  return Type(mtype, ftype);
}

template <>
inline auto getSimpleType<signed short>() {
  BasicType mtype(H5T_NATIVE_SHORT, false);
  BasicType ftype(detail::getRawInt<signed short>(), false);

  return Type(mtype, ftype);
}

template <>
inline auto getSimpleType<unsigned int>() {
  BasicType mtype(H5T_NATIVE_UINT, false);
  BasicType ftype(detail::getRawInt<unsigned int>(), false);

  return Type(mtype, ftype);
}

template <>
inline auto getSimpleType<signed int>() {
  BasicType mtype(H5T_NATIVE_INT, false);
  BasicType ftype(detail::getRawInt<signed int>(), false);

  return Type(mtype, ftype);
}

template <>
inline auto getSimpleType<unsigned long>() {
  BasicType mtype(H5T_NATIVE_ULONG, false);
  BasicType ftype(detail::getRawInt<unsigned long>(), false);

  return Type(mtype, ftype);
}

template <>
inline auto getSimpleType<signed long>() {
  BasicType mtype(H5T_NATIVE_LONG, false);
  BasicType ftype(detail::getRawInt<signed long>(), false);

  return Type(mtype, ftype);
}

template <>
inline auto getSimpleType<unsigned long long>() {
  BasicType mtype(H5T_NATIVE_ULLONG, false);
  BasicType ftype(detail::getRawInt<unsigned long long>(), false);

  return Type(mtype, ftype);
}

template <>
inline auto getSimpleType<signed long long>() {
  BasicType mtype(H5T_NATIVE_LLONG, false);
  BasicType ftype(detail::getRawInt<signed long long>(), false);

  return Type(mtype, ftype);
}

// Float types
template <>
inline auto getSimpleType<float>() {
  BasicType mtype(H5T_NATIVE_FLOAT, false);
  BasicType ftype(H5T_IEEE_F32LE, false);

  return Type(mtype, ftype);
}

template <>
inline auto getSimpleType<double>() {
  BasicType mtype(H5T_NATIVE_DOUBLE, false);
  BasicType ftype(H5T_IEEE_F64LE, false);

  return Type(mtype, ftype);
}

// Other types
template <>
inline auto getSimpleType<bool>() {
  bool truthy = true;
  bool falsy = false;

  return detail::getIntBoolType(sizeof(bool), &truthy, &falsy);
}

} // namespace f5

#endif
