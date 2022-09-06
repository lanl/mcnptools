#ifndef SHACL_F5_H5DATASET_UTILITY_HPP
#define SHACL_F5_H5DATASET_UTILITY_HPP

#include <stdexcept>
#include <string>

#include "shacl/f5/hdf5_nowarnings.hpp"

#include "shacl/f5/H5Dataset/DatasetBuilder.hpp"
#include "shacl/f5/H5Dataset/DatasetType.hpp"

namespace f5 {

// Open datasets
inline Dataset openH5Dataset(hid_t parent_id, const std::string &name) {
  // Dataset
  auto dataset_id = H5Dopen(parent_id, name.c_str(), H5P_DEFAULT);
  if (dataset_id < 0) {
    throw std::runtime_error("Unable to open HDF5 dataset.");
  }

  // TODO(cjosey) : determine how severe the performance penalty in parallel
  // is to read the metadata from the file.

  // Dataspace
  auto file_space = detail::Dataspace(H5Dget_space(dataset_id));

  // Type
  auto file_type_id = H5Tcopy(dataset_id);
  auto native_type_id = H5Tget_native_type(file_type_id, H5T_DIR_ASCEND);
  auto ftype = BasicType(file_type_id, true);
  auto mtype = BasicType(native_type_id, true);
  auto type = Type(mtype, ftype);
  return Dataset(dataset_id, type, file_space);
}

inline Dataset openH5Dataset(hid_t parent_id, const std::string &name,
                             Type type) {
  auto dataset_id = H5Dopen(parent_id, name.c_str(), H5P_DEFAULT);
  if (dataset_id < 0) {
    throw std::runtime_error("Unable to open HDF5 dataset.");
  }

  auto file_space = detail::Dataspace(H5Dget_space(dataset_id));

  return Dataset(dataset_id, std::move(type), file_space);
}

} // namespace f5

#endif
