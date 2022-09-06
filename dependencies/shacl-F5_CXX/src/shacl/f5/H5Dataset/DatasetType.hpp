#ifndef SHACL_F5_H5DATASET_DATASETTYPE_HPP
#define SHACL_F5_H5DATASET_DATASETTYPE_HPP

#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include "shacl/f5/hdf5_nowarnings.hpp"

#include "shacl/f5/H5Attribute.hpp"
#include "shacl/f5/H5Dataspace.hpp"
#include "shacl/f5/H5Holders.hpp"
#include "shacl/f5/H5Properties.hpp"
#include "shacl/f5/H5Type.hpp"

namespace f5 {

static const auto default_transfer_props = PropertiesHandle{H5P_DEFAULT, false};

class Dataset : public Handle<H5Dclose> {
private:
  // Other components
  Type type_;
  detail::Dataspace file_space_;

  template <typename T>
  void write(const T *data, const hid_t memory_id, const hid_t file_id,
             const hid_t properties_id) const {
    static_assert(
        !std::is_same<std::string, T>::value,
        "use a std::vector<std::string> to write to a string dataset");

    auto status = H5Dwrite(this->getID(), type_.memory_type.getID(), memory_id,
                           file_id, properties_id, data);

    if (status != 0) {
      throw std::runtime_error("Unable to write to HDF5 dataset.");
    }
  }

  template <typename T>
  void read(T *data, const hid_t memory_id, const hid_t file_id,
            const hid_t properties_id) const {
    static_assert(
        !std::is_same<std::string, T>::value,
        "use a std::vector<std::string> to read from a string dataset");

    auto status = H5Dread(this->getID(), type_.memory_type.getID(), memory_id,
                          file_id, properties_id, data);

    if (status != 0) {
      throw std::runtime_error("Unable to read from HDF5 dataset.");
    }
  }

  void checkConsistency(const std::vector<hsize_t> &shape,
                        const std::vector<hsize_t> &offset,
                        const std::string &verb) const {
    size_t rank = file_space_.rank();

    // Check first that the ranks are all the same.
    if (shape.size() != offset.size() && shape.size() != rank) {
      throw std::logic_error(verb + " with incompatible dimensions.");
    }

    // Then check we're not falling off the edge
    for (size_t i = 0; i < rank; ++i) {
      if (shape[i] + offset[i] > this->shape()[i]) {
        throw std::logic_error(verb + " beyond the edge of the dataset.");
      }
    }
  }

public:
  Dataset(const hid_t id, Type type, detail::Dataspace file_space)
      : Handle(id), type_(std::move(type)), file_space_(std::move(file_space)) {
  }

  // Read/Write bare pointers to data
  template <typename T>
  void write(const T *data) const {
    write(data, H5S_ALL, H5S_ALL, H5P_DEFAULT);
  }

  template <typename T>
  void read(T *data) const {
    read(data, H5S_ALL, H5S_ALL, H5P_DEFAULT);
  }

  // Read/Write for STL containers
  template <typename array_t,
            std::enable_if_t<!std::is_pointer<array_t>::value, bool> = true>
  void write(const array_t &data) const {
    static_assert(!std::is_same<bool, typename array_t::value_type>::value,
                  "Vectors of bools are not guaranteed contiguous.");

    if (this->size() != data.size()) {
      throw std::runtime_error("Writing beyond the edge of the dataset.");
    }

    write(data.data());
  }

  // Read/write vectors of strings
  void write(const std::vector<std::string> &data) const {

    // Create a raw c pointer array of the data without
    // copying the underlying char data
    std::vector<const char *> c_strings;
    c_strings.reserve(data.size());
    for (auto &str : data) {
      c_strings.emplace_back(str.c_str());
    }

    write(c_strings.data());
  }

  void read(std::vector<std::string> &data) const {

    // Specify how to read raw data
    auto read_data = [this](const auto &type, auto *data) {
      return H5Dread(this->getID(), type.memory_type.getID(), H5S_ALL, H5S_ALL,
                     H5P_DEFAULT, data);
    };

    // Get the file-type from dataset which may not match std::string
    BasicType file_type{H5Dget_type(this->getID()), true};

    detail::readStringData(*this, this->file_space_, file_type, read_data,
                           data);
  }

  template <typename array_t>
  void read(array_t &data) const {
    static_assert(!std::is_same<bool, typename array_t::value_type>::value,
                  "Vectors of bools are not guaranteed contiguous.");

    // Resize data to match dataset
    data.resize(this->size());

    read(data.data());
  }

  // Read/Write bare pointers on hyperslabs
  template <typename T>
  void
  write(const T *data, const std::vector<hsize_t> &offset,
        const std::vector<hsize_t> &shape,
        const PropertiesHandle &properties = default_transfer_props) const {
    checkConsistency(shape, offset, "Writing");

    // Create memory space, select hyperslab on main space to match
    auto memory_space = detail::Dataspace(shape);
    auto file_space = detail::Dataspace(H5Scopy(file_space_.getID()));
    if (file_space.rank() != 0) {
      file_space.setHyperslab(shape, offset);
    }

    write(data, memory_space.getID(), file_space.getID(), properties.getID());
  }

  template <typename T>
  void read(T *data, const std::vector<hsize_t> &offset,
            const std::vector<hsize_t> &shape,
            const PropertiesHandle &properties = default_transfer_props) const {
    checkConsistency(shape, offset, "Reading");

    // Select hyperslab if not scalar
    auto memory_space = detail::Dataspace(shape);
    auto file_space = detail::Dataspace(H5Dget_space(this->getID()));
    if (file_space.rank() != 0) {
      file_space.setHyperslab(shape, offset);
    }

    read(data, memory_space.getID(), file_space.getID(), properties.getID());
  }

  // Read/Write STL on hyperslabs
  template <typename array_t>
  void
  write(const std::vector<array_t> &data, size_t offset, size_t size,
        const PropertiesHandle &properties = default_transfer_props) const {
    static_assert(!std::is_same<bool, array_t>::value,
                  "Vectors of bools are not guaranteed contiguous.");
    static_assert(!std::is_same<std::string, array_t>::value,
                  "There is no support for slices of vectors of strings.");

    if (size > data.size()) {
      throw std::logic_error(
          "Attempting to write vector with size smaller than given size.");
    }

    write(data.data(), {offset}, {size}, properties);
  }

  template <typename array_t>
  void read(std::vector<array_t> &data, size_t offset, size_t size,
            const PropertiesHandle &properties = default_transfer_props) const {
    static_assert(!std::is_same<bool, array_t>::value,
                  "Vectors of bools are not guaranteed contiguous.");
    static_assert(!std::is_same<std::string, array_t>::value,
                  "There is no support for slices of vectors of strings.");

    data.resize(size);

    read(data.data(), {offset}, {size}, properties);
  }

  // Handle attributes
  template <typename... Args>
  void writeAttribute(const std::string &s, Args &&... args) const {
    writeH5Attribute(this->getID(), s, std::forward<Args>(args)...);
  }

  template <typename... Args>
  void readAttribute(const std::string &s, Args &&... args) const {
    readH5Attribute(this->getID(), s, std::forward<Args>(args)...);
  }

  bool checkAttribute(const std::string &s) const {
    return checkH5Attribute(this->getID(), s);
  }

  void deleteAttribute(const std::string &s) const {
    deleteH5Attribute(this->getID(), s);
  }

  // Support resizing arrays
  void reshape(const std::vector<hsize_t> &new_shape) {
    // Check first that this can be done
    size_t n = this->shape().size();
    if (new_shape.size() != n) {
      throw std::logic_error("Resizing with incompatible dimensions.");
    }

    for (size_t i = 0; i < n; ++i) {
      size_t max_extent = this->maxShape()[i];
      if (new_shape[i] > max_extent && max_extent != H5S_UNLIMITED) {
        throw std::invalid_argument(
            "New shape larger than maximum shape on dimension " +
            std::to_string(i) + ".");
      }
    }

    H5Dset_extent(this->getID(), new_shape.data());

    // Update the dataset
    file_space_ = detail::Dataspace(H5Dget_space(this->getID()));
  }

  const std::vector<hsize_t> &shape() const {
    return file_space_.shape();
  }

  hsize_t size() const {
    return file_space_.size();
  }

  const std::vector<hsize_t> &maxShape() const {
    return file_space_.maxShape();
  }
};

} // namespace f5

#endif
