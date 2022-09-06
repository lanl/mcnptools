#ifndef SHACL_F5_H5DATASPACE_HPP
#define SHACL_F5_H5DATASPACE_HPP

#include <functional>
#include <memory>
#include <numeric>
#include <stdexcept>
#include <string>
#include <vector>

#include "shacl/f5/hdf5_nowarnings.hpp"

#include "shacl/f5/H5Holders.hpp"

namespace f5 {

namespace detail {

inline hid_t getDataspace(const std::vector<hsize_t> &shape,
                          const std::vector<hsize_t> &max_shape) {
  // Check consistency of shapes
  size_t rank = shape.size();
  if (max_shape.size() != rank) {
    throw std::logic_error(
        "Max shape dimension is inconsistent with file shape.");
  }
  for (size_t i = 0; i < rank; ++i) {
    if (shape[i] > max_shape[i] && max_shape[i] != H5S_UNLIMITED) {
      throw std::invalid_argument(
          "File shape larger than maximum shape on dimension " +
          std::to_string(i) + ".");
    }
  }

  auto id =
      H5Screate_simple(static_cast<int>(rank), shape.data(), max_shape.data());

  if (id < 0) {
    throw std::runtime_error("Unable to create HDF5 dataspace.");
  }

  return id;
}

class Dataspace : public Handle<H5Sclose> {
private:
  size_t rank_ = 0;
  std::vector<hsize_t> file_shape_;
  std::vector<hsize_t> max_file_shape_;
  hsize_t size_;

public:
  // Create one from a known ID
  explicit Dataspace(hid_t id)
      : Handle(id), rank_(H5Sget_simple_extent_ndims(id)),
        file_shape_(rank_, 0), max_file_shape_(rank_, 0), size_(0) {
    H5Sget_simple_extent_dims(this->getID(), file_shape_.data(),
                              max_file_shape_.data());

    size_ =
        std::accumulate(file_shape_.begin(), file_shape_.end(),
                        static_cast<hsize_t>(1), std::multiplies<hsize_t>());
  }

  // Create one from a desired shape and maximum shape
  Dataspace(std::vector<hsize_t> shape, std::vector<hsize_t> max_shape)
      : Handle(getDataspace(shape, max_shape)) {
    // Check consistency of shapes
    rank_ = shape.size();
    if (max_shape.size() != rank_) {
      throw std::logic_error(
          "Max shape dimension is inconsistent with file shape.");
    }
    for (size_t i = 0; i < rank_; ++i) {
      if (shape[i] > max_shape[i] && max_shape[i] != H5S_UNLIMITED) {
        throw std::invalid_argument(
            "File shape larger than maximum shape on dimension " +
            std::to_string(i) + ".");
      }
    }
    file_shape_ = std::move(shape);
    max_file_shape_ = std::move(max_shape);

    size_ =
        std::accumulate(file_shape_.begin(), file_shape_.end(),
                        static_cast<hsize_t>(1), std::multiplies<hsize_t>());
  }

  // Create one from a desired shape
  explicit Dataspace(const std::vector<hsize_t> &shape)
      : Dataspace(shape, shape) {}

  void setHyperslab(const std::vector<hsize_t> &shape,
                    const std::vector<hsize_t> &offset) {
    // Selects a hyperslab starting at offset with a given shape

    // TODO(cjosey) : consider if people need more advanced indexing
    std::vector<hsize_t> stride(rank_, 1);
    std::vector<hsize_t> count(rank_, 1);

    H5Sselect_hyperslab(this->getID(), H5S_SELECT_SET, offset.data(),
                        stride.data(), count.data(), shape.data());

    size_ = std::accumulate(shape.begin(), shape.end(), static_cast<hsize_t>(1),
                            std::multiplies<hsize_t>());
  }

  const std::vector<hsize_t> &shape() const {
    return file_shape_;
  }

  hsize_t size() const {
    return size_;
  }

  const std::vector<hsize_t> &maxShape() const {
    return max_file_shape_;
  }

  size_t rank() const {
    return rank_;
  }
};

} // namespace detail

} // namespace f5

#endif
