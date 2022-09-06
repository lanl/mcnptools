#ifndef SHACL_F5_H5DATASET_READ_HPP
#define SHACL_F5_H5DATASET_READ_HPP

#include <stdexcept>
#include <string>
#include <vector>

#include "shacl/f5/hdf5_nowarnings.hpp"

#include "shacl/f5/H5Dataset/DatasetBuilder.hpp"
#include "shacl/f5/H5Dataset/DatasetType.hpp"

namespace f5 {

template <typename array_t>
Dataset readH5Dataset(const hid_t parent_id, const std::string &name,
                      array_t &data, const Type &type) {
  auto dataset = openH5Dataset(parent_id, name, type);
  dataset.read(data);

  return dataset;
}

template <typename array_t>
Dataset readH5Dataset(const hid_t parent_id, const std::string &name,
                      array_t &data) {
  auto dataset = openH5Dataset(parent_id, name);
  dataset.read(data);

  return dataset;
}

template <typename array_t>
Dataset readH5Dataset(const hid_t parent_id, const std::string &name,
                      array_t &data, const Type &type,
                      const PropertiesHandle &properties) {
  auto dataset = openH5Dataset(parent_id, name, type);
  dataset.read(data, 0, data.size(), properties);

  return dataset;
}

template <typename array_t>
Dataset readH5Dataset(const hid_t parent_id, const std::string &name,
                      array_t &data, const PropertiesHandle &properties) {
  auto dataset = openH5Dataset(parent_id, name);
  dataset.read(data, 0, data.size(), properties);

  return dataset;
}

} // namespace f5

#endif
