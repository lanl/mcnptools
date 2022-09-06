#ifndef SHACL_F5_H5DATASET_WRITE_HPP
#define SHACL_F5_H5DATASET_WRITE_HPP

#include <stdexcept>
#include <string>
#include <vector>

#include "shacl/f5/hdf5_nowarnings.hpp"

#include "shacl/f5/H5Dataset/DatasetBuilder.hpp"
#include "shacl/f5/H5Dataset/DatasetType.hpp"

namespace f5 {

// Dataset IO for files/groups
template <typename data_t, typename type_t>
Dataset writeH5Dataset(const hid_t parent_id, const std::string &name,
                       const data_t *data, const type_t &type,
                       const std::vector<hsize_t> &shape) {
  auto dataset =
      DatasetBuilder().setShape(shape).setType(type).build(parent_id, name);
  dataset.write(data);

  return dataset;
}

// Dataset IO for STL containers with "size", "value_type", and "data"
template <typename array_t>
Dataset writeH5Dataset(const hid_t parent_id, const std::string &name,
                       const array_t &data) {
  auto dataset = DatasetBuilder().inferFrom(data).build(parent_id, name);
  dataset.write(data);

  return dataset;
}

// Dataset IO for STL containers, type overloaded
template <typename array_t, typename type_t>
Dataset writeH5Dataset(const hid_t parent_id, const std::string &name,
                       const array_t &data, const type_t &type) {
  auto dataset = DatasetBuilder()
                     .setType(type)
                     .setShape({data.size()})
                     .build(parent_id, name);
  dataset.write(data);

  return dataset;
}

} // namespace f5

#endif
