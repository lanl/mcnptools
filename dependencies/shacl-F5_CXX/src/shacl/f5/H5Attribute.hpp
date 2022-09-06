#ifndef SHACL_F5_H5ATTRIBUTE_HPP
#define SHACL_F5_H5ATTRIBUTE_HPP

#include <algorithm>
#include <array>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <vector>

#include "shacl/f5/hdf5_nowarnings.hpp"

#include "shacl/f5/H5Dataspace.hpp"
#include "shacl/f5/H5Type.hpp"
#include "shacl/f5/StringSupport.hpp"

namespace f5 {

namespace detail {
template <typename data_t>
void readH5AttributeImpl(const AttributeHandle &attribute, data_t *data,
                         const Type &type) {
  if (attribute.getID() < 0) {
    throw std::runtime_error("Unable to open HDF5 attribute.");
  }

  auto status = H5Aread(attribute.getID(), type.memory_type.getID(), data);

  if (status < 0) {
    throw std::runtime_error("Unable to read HDF5 attribute.");
  }
}
} // namespace detail

template <typename data_t>
void writeH5Attribute(hid_t parent_id, const std::string &name,
                      const data_t *data, const Type &type,
                      const std::vector<hsize_t> &shape) {
  // Raw pointer writer for HDF5

  // Create the dataspace
  auto dataspace =
      H5Screate_simple(static_cast<int>(shape.size()), shape.data(), nullptr);

  if (dataspace < 0) {
    throw std::runtime_error("Unable to create HDF5 dataspace.");
  }

  // Create attribute
  AttributeHandle attribute{H5Acreate(parent_id, name.c_str(),
                                      type.file_type.getID(), dataspace,
                                      H5P_DEFAULT, H5P_DEFAULT)};

  if (attribute.getID() < 0) {
    throw std::runtime_error("Unable to create HDF5 attribute.");
  }

  auto status = H5Awrite(attribute.getID(), type.memory_type.getID(), data);

  if (status < 0) {
    throw std::runtime_error("Unable to write HDF5 attribute.");
  }

  // Close things
  H5Sclose(dataspace);
}

template <typename data_t>
void readH5Attribute(hid_t parent_id, const std::string &name, data_t *data,
                     const Type &type) {
  // Free function used by Fortran
  const auto attribute =
      AttributeHandle{H5Aopen(parent_id, name.c_str(), H5P_DEFAULT)};
  detail::readH5AttributeImpl(attribute, data, type);
}

template <typename T>
void readH5Attribute(hid_t parent_id, const std::string &name,
                     std::vector<T> &data) {

  static_assert(!std::is_same<bool, T>::value,
                  "Vectors of bools are not guaranteed contiguous.");

  // Helper code for 1D containers
  const auto type = getSimpleType<T>();

  // Resize the container
  const auto attribute =
      AttributeHandle{H5Aopen(parent_id, name.c_str(), H5P_DEFAULT)};
  detail::Dataspace space{H5Aget_space(attribute.getID())};
  const auto size = space.size();
  data.resize(size);

  detail::readH5AttributeImpl(attribute, data.data(), type);
}

template <typename scalar_t,
          std::enable_if_t<std::is_scalar<scalar_t>::value, bool> = true,
          std::enable_if_t<!std::is_pointer<scalar_t>::value, bool> = true>
void readH5Attribute(hid_t parent_id, const std::string &name,
                     scalar_t &value) {
  const auto type = getSimpleType<scalar_t>();
  const auto attribute =
      AttributeHandle{H5Aopen(parent_id, name.c_str(), H5P_DEFAULT)};

  detail::readH5AttributeImpl(attribute, &value, type);
}

template <typename array_t,
          std::enable_if_t<!std::is_scalar<array_t>::value, bool> = true,
          std::enable_if_t<!std::is_constructible<std::string, array_t>::value,
                           bool> = true>
void writeH5Attribute(hid_t parent_id, const std::string &name,
                      const array_t &data) {
  static_assert(!std::is_same<std::vector<bool>, array_t>::value,
                  "Vectors of bools are not guaranteed contiguous.");


  // Helper code for 1D containers
  std::vector<hsize_t> shape = {data.size()};
  auto type = getSimpleType<typename array_t::value_type>();
  writeH5Attribute(parent_id, name, data.data(), type, shape);
}

template <typename scalar_t,
          std::enable_if_t<std::is_scalar<scalar_t>::value, bool> = true,
          std::enable_if_t<!std::is_pointer<scalar_t>::value, bool> = true>
void writeH5Attribute(hid_t parent_id, const std::string &name,
                      const scalar_t value) {
  // Helper code for scalar values
  std::array<scalar_t, 1> data{value};
  writeH5Attribute(parent_id, name, data);
}

inline void writeH5Attribute(hid_t parent_id, const std::string &name,
                             const std::vector<std::string> &data) {
  // Create a raw c pointer array of the data without
  // copying the underlying char data
  std::vector<const char *> c_strings;
  c_strings.reserve(data.size());
  for (auto &str : data) {
    c_strings.emplace_back(str.c_str());
  }
  auto type = getSimpleType<std::string>();
  std::vector<hsize_t> shape = {c_strings.size()};
  writeH5Attribute(parent_id, name, c_strings.data(), type, shape);
}

// Explicitly handle std::strings as an H5str, rather than as an array of chars
template <
    typename stringlike_t,
    std::enable_if_t<std::is_constructible<std::string, stringlike_t>::value,
                     bool> = true>
void writeH5Attribute(hid_t parent_id, const std::string &name,
                      const stringlike_t &value) {
  // Write string data directly to avoid an extra allocation
  // that would be required to construct a string
  std::array<const char *, 1> data{detail::asCString(value)};
  std::vector<hsize_t> shape = {data.size()};
  const auto type = getSimpleType<std::string>();

  writeH5Attribute(parent_id, name, data.data(), type, shape);
}

inline void readH5Attribute(hid_t parent_id, const std::string &name,
                            std::vector<std::string> &values) {
  AttributeHandle attribute{H5Aopen(parent_id, name.c_str(), H5P_DEFAULT)};
  detail::Dataspace space{H5Aget_space(attribute.getID())};

  //Check dimensionality is 1 for anything that is non-scalar:
  if (space.rank() > 1) {
    throw std::runtime_error(
          "Higher-dimensional string datasets are not supported currently");
  }

  BasicType file_type{H5Aget_type(attribute.getID()), true};
  auto read_data = [&](const auto &type, auto *data) {
    return H5Aread(attribute.getID(), type.memory_type.getID(), data);
  };

  detail::readStringData(attribute, space, file_type, read_data, values);
}

inline void readH5Attribute(hid_t parent_id, const std::string &name,
                            std::string &value) {
  // Forward to vector read
  std::vector<std::string> array_values;
  readH5Attribute(parent_id, name, array_values);

  if (array_values.size() > 1) {
    throw std::runtime_error("Expected a single string in dataset");
  }
  value = array_values.empty() ? "" : std::move(array_values[0]);
}

inline bool checkH5Attribute(hid_t id, const std::string &name) {
  return (H5Aexists(id, name.c_str()) > 0);
}

inline void deleteH5Attribute(hid_t id, const std::string &name) {
  if (!checkH5Attribute(id, name)) {
    throw std::runtime_error(
        "There was a problem attempting to delete attribute \"" + name + "\".");
  }
  H5Adelete(id, name.c_str());
}

} // namespace f5

#endif
