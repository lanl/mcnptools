#ifndef SHACL_F5_H5DATASET_DATASETBUILDER_HPP
#define SHACL_F5_H5DATASET_DATASETBUILDER_HPP

#include <algorithm>
#include <numeric>
#include <stdexcept>
#include <string>
#include <vector>

#include "shacl/f5/hdf5_nowarnings.hpp"

#include "shacl/f5/H5Dataset/DatasetType.hpp"
#include "shacl/f5/H5Dataspace.hpp"
#include "shacl/f5/H5Properties.hpp"
#include "shacl/f5/H5Type.hpp"

namespace f5 {

// Default compression is 1 for scientific software that care more about
// throughput than compression.  Users can of course override.
const int DEFAULT_COMPRESSION = 1;

class DatasetBuilder {
  std::vector<hsize_t> file_shape_;
  bool shape_set_ = false;

  std::vector<hsize_t> max_file_shape_;
  bool max_shape_set_ = false;

  std::vector<hsize_t> chunk_shape_;
  bool chunk_set_ = false;
  bool chunk_auto_ = false;

  int compression_level_ = DEFAULT_COMPRESSION;
  bool compression_set_ = false;

  Type type_;
  bool type_set_ = false;

public:
  DatasetBuilder() = default;

  template <typename array_t> DatasetBuilder &inferFrom(array_t data) {
    type_ = getSimpleType<typename array_t::value_type>();
    type_set_ = true;

    file_shape_ = {data.size()};
    shape_set_ = true;

    return *this;
  }

  DatasetBuilder &setShape(std::vector<hsize_t> shape) {
    file_shape_ = std::move(shape);
    shape_set_ = true;

    return *this;
  }

  DatasetBuilder &setMaxShape(std::vector<hsize_t> max_shape) {
    max_file_shape_ = std::move(max_shape);
    max_shape_set_ = true;

    return *this;
  }

  DatasetBuilder &setType(Type type) {
    type_ = std::move(type);
    type_set_ = true;

    return *this;
  }

  DatasetBuilder &
  enableCompression(int compression_level = DEFAULT_COMPRESSION) {
    compression_level_ = compression_level;
    compression_set_ = true;

    return *this;
  }

  DatasetBuilder &setAutoChunk() {
    if (chunk_set_) {
      throw std::logic_error(
          "Cannot set automatic chunking with explicit chunking.");
    }
    chunk_auto_ = true;
    chunk_set_ = true;

    return *this;
  }

  DatasetBuilder &setChunkShape(std::vector<hsize_t> chunk_shape) {
    if (chunk_auto_) {
      throw std::logic_error(
          "Cannot set explicit chunking with automatic chunking.");
    }
    chunk_shape_ = std::move(chunk_shape);
    chunk_set_ = true;

    return *this;
  }

  Dataset build(hid_t parent_id, const std::string &name) const {
    // Completeness checks
    if (!type_set_) {
      throw std::logic_error("Cannot create dataset without a type.");
    }
    if (!shape_set_) {
      throw std::logic_error("Cannot create dataset without a file shape.");
    }

    // Set up properties
    auto properties = detail::Properties();

    if (chunk_set_) {
      if (compression_set_) {
        properties.enableCompression(compression_level_);
      }

      // Assign automatic chunking, as appropriate.  If the array has zero
      // elements, do not calculate auto chunking shapes to avoid divide-by-zero
      // issues.  The accumulate is computing the product of the dimensions to
      // identify the number of elements present.
      std::vector<hsize_t> chunk_shape(file_shape_.size(), 1);
      chunk_shape = chunk_auto_ ? (
        std::accumulate(
          file_shape_.begin(),
          file_shape_.end(),
          static_cast<hsize_t>(1),
          std::multiplies<hsize_t>()) > 0 ?
        calculateAutoChunkShape(chunk_shape) : chunk_shape
        ) : chunk_shape_;
      if (chunk_shape.size() != file_shape_.size()) {
        throw std::logic_error(
            "The chunk shape dimensions are inconsistent with the file shape.");
      }
      properties.setChunkShape(chunk_shape);
    } else if (compression_set_) {
      throw std::logic_error("Compression cannot be used without chunking.");
    }

    std::vector<hsize_t> max_file_shape;

    // Set up dataspace
    if (max_shape_set_) {
      // If resizable, chunking required.
      if (!chunk_set_ && max_file_shape_ != file_shape_) {
        throw std::logic_error(
            "Cannot make a resizable array without chunking.");
      }
      max_file_shape = max_file_shape_;
    } else {
      max_file_shape = file_shape_;
    }

    // Shape / max shape consistency checked in Dataspace
    auto file_space = detail::Dataspace(file_shape_, max_file_shape);

    // Set up dataset
    hid_t dataset_id = H5Dcreate(
        parent_id, name.c_str(), type_.file_type.getID(), file_space.getID(),
        detail::utf8LinkProperty().getID(), properties.getID(), H5P_DEFAULT);

    if (dataset_id < 0) {
      throw std::runtime_error("Unable to create HDF5 dataset.");
    }

    return Dataset(dataset_id, type_, file_space);
  }

  template <class GroupLike>
  Dataset build(const GroupLike &parent, const std::string &name) const {
    return build(parent.getID(), name);
  }

  size_t calculateLastChunk(const size_t &last_dim,
                            const size_t &last_chunk) const {
    // Already optimal if a prime factor.
    if (last_dim % last_chunk == 0) {
      return last_chunk;
    }

    // Iterate to a maximum of 50, which gives a maximum 2% slop in chunk size.
    const size_t max_division = 50;
    size_t end = std::min(max_division, last_dim / 2);
    for (size_t i = last_dim / last_chunk + 1; i <= end; ++i) {
      if (last_dim % i == 0) {
        return last_dim / i;
      }
    }

    // A better solution was not (quickly) found.
    return last_chunk;
  }

private:
  std::vector<hsize_t> calculateAutoChunkShape( const std::vector<hsize_t> & s ) const {
    // Chunk shape specification, to be automatically populated and returned.
    std::vector<hsize_t> chunk_shape(s);
    // Comparator value to ensure non-zero chunk size.
    const hsize_t one = 1;
    // Compute target chunk size in number of elements based on the size of the
    // type of data being stored in the dataset. Typical HDF5 cache sizes are
    // 1M, so the chunk size in bytes is specified somewhat below that
    // (1,000,000 vs. 1,048,576 bytes).
    const auto chunk_target_size = 1000000 / type_.file_type.getSize();
    // Set a naive target chunk size in array elements.
    size_t chunk_remaining = chunk_target_size;

    // Identify chunking along each of the ranks.
    size_t n = file_shape_.size();
    while (n > 0) {
      --n;
      if (chunk_remaining >= file_shape_[n]) {
        chunk_shape[n] = file_shape_[n];
      } else {
        chunk_shape[n] = std::max(
            static_cast<hsize_t>(chunk_remaining % file_shape_[n]), one);
      }
      chunk_remaining =
          std::max(static_cast<hsize_t>(chunk_remaining / file_shape_[n]), one);
      if (chunk_remaining == one) {
        break;
      }
    }

    // Factor last chunked rank to fit into memory efficiently while maintaining
    // calculational performance.
    chunk_shape[n] = calculateLastChunk(file_shape_[n], chunk_shape[n]);

    return chunk_shape;
  }
};

} // namespace f5

#endif
