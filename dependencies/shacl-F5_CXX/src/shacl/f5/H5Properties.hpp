#ifndef SHACL_F5_H5PROPERTIES_HPP
#define SHACL_F5_H5PROPERTIES_HPP

#include <memory>
#include <stdexcept>
#include <vector>

#include "shacl/f5/hdf5_nowarnings.hpp"

#include "shacl/f5/H5Holders.hpp"

namespace f5 {

namespace detail {

const int MIN_COMPRESSION = 1;
const int MAX_COMPRESSION = 9;

class Properties : public PropertiesHandle {
public:
  // Default to default properties
  explicit Properties() : Handle(H5Pcreate(H5P_DATASET_CREATE)) {}

  Properties &enableCompression(int compression_level) {
    // Attempts to enable compression.
    if (compression_level < MIN_COMPRESSION ||
        compression_level > MAX_COMPRESSION) {
      throw std::invalid_argument("Compression level is invalid.");
    }

    if (H5Zfilter_avail(H5Z_FILTER_DEFLATE) > 0 &&
        H5Zfilter_avail(H5Z_FILTER_SHUFFLE) > 0) {
      uint32_t deflate_info;
      uint32_t shuffle_info;
      auto status = H5Zget_filter_info(H5Z_FILTER_DEFLATE, &deflate_info);

      if (status < 0) {
        throw std::runtime_error("Unable to query HDF5 deflate filter.");
      }

      status = H5Zget_filter_info(H5Z_FILTER_SHUFFLE, &shuffle_info);

      if (status < 0) {
        throw std::runtime_error("Unable to query HDF5 shuffle filter.");
      }

      if ((deflate_info & H5Z_FILTER_CONFIG_ENCODE_ENABLED) != 0U &&
          (deflate_info & H5Z_FILTER_CONFIG_DECODE_ENABLED) != 0U &&
          (shuffle_info & H5Z_FILTER_CONFIG_ENCODE_ENABLED) != 0U &&
          (shuffle_info & H5Z_FILTER_CONFIG_DECODE_ENABLED) != 0U) {

        H5Pset_shuffle(this->getID());
        H5Pset_deflate(this->getID(), compression_level);
      }
    }

    return *this;
  }

  Properties &setChunkShape(const std::vector<hsize_t> &chunk_shape) {
    // Enables chunking.
    H5Pset_chunk(this->getID(), static_cast<int>(chunk_shape.size()),
                 chunk_shape.data());

    return *this;
  }
};

// Property to indicate that created links (groups/datasets) are UTF-8 encoded
inline auto utf8LinkProperty() {
  auto utf8_link_id = H5Pcreate(H5P_LINK_CREATE);
  H5Pset_char_encoding(utf8_link_id, H5T_CSET_UTF8);
  return PropertiesHandle{utf8_link_id};
}

} // namespace detail

} // namespace f5

#endif
