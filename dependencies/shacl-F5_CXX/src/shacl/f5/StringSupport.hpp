#ifndef SHACL_F5_STRINGSUPPORT_HPP
#define SHACL_F5_STRINGSUPPORT_HPP

#include <algorithm>
#include <string>

#include "shacl/f5/H5Dataspace.hpp"
#include "shacl/f5/H5Type.hpp"

namespace f5 {

namespace detail {
// Adapt string-like objects to avoid allocating c_strings.
inline char const *asCString(const std::string &string) {
  return string.c_str();
}

inline char const *asCString(const char *c_string) {
  return c_string;
}

template <typename Handle, typename Reader>
void readStringData(const Handle &handle, const Dataspace &space,
                    const BasicType &file_type, const Reader &reader,
                    std::vector<std::string> &values) {
  if (handle.getID() < 0) {
    throw std::runtime_error("Unable to open HDF5 handle.");
  }

  // Check for variable size and pad char
  const bool variable = H5Tis_variable_str(file_type.getID());
  auto string_size = H5Tget_size(file_type.getID());
  auto string_strpad = H5Tget_strpad(file_type.getID());

  if ( H5Tget_cset(file_type.getID()) != H5T_CSET_ASCII) {
    throw std::runtime_error("F5 currently only supports reading strings encoded with the ASCII character set, not UTF-8"); 
  }

  const auto length = space.size();

  // Get correct memory type to read into
  const auto type = (variable) ? getSimpleType<std::string>()
                               : getFixedStringType(string_size, string_strpad);

  if (variable) {
    std::vector<char *> c_strings{length};
    auto status = reader(type, c_strings.data());
    if (status < 0) {
      throw std::runtime_error("Unable to read HDF5 variable string data.");
    }

    values.clear();
    values.reserve(c_strings.size());
    values = {c_strings.begin(), c_strings.end()};

    // Free the memory used by HDF5 to represent variable length strings
    if (!c_strings.empty()) {
      H5Dvlen_reclaim(type.memory_type.getID(), space.getID(), H5P_DEFAULT,
                      c_strings.data());
    }
  } else {

    // Manually unflatten fixed-size, padded (or terminated) strings.  This
    // can be viewed as flattened 2D array of data
    std::vector<char> raw_strings;
    raw_strings.resize(string_size * length);
    auto status = reader(type, raw_strings.data());

    if (status < 0) {
      throw std::runtime_error("Unable to read HDF5 fixed-string data.");
    }

    const char pad_char = detail::getStringPadChar(string_strpad);
    values.clear();
    values.reserve(length);
    using Diff_t = decltype(raw_strings.end() - raw_strings.begin());
    const Diff_t width = static_cast<Diff_t>(string_size);
    for (size_t string_idx = 0; string_idx < length; ++string_idx) {
      const auto start = raw_strings.begin() + string_idx * width;
      const auto end = start + width;
      values.emplace_back(start, end); // Create string in-place
    }

    // Remove any padding on right
    for (auto &str : values) {
      auto end = std::find_if(str.rbegin(), str.rend(), [=](auto c) {
                   return c != pad_char;
                 }).base();
      str.erase(end, str.end());
    }
  }
}
} // namespace detail
} // namespace f5

#endif
