#ifndef SHACL_F5_H5HOLDERS_HPP
#define SHACL_F5_H5HOLDERS_HPP

#include <memory>
#include <type_traits>

#include "shacl/f5/hdf5_nowarnings.hpp"

namespace f5 {

namespace detail {

template <herr_t close_policy(hid_t)>
class Holder {
private:
  hid_t hid;

public:
  explicit Holder(hid_t id) : hid(id) {}
  Holder(const Holder &) = default;
  Holder &operator=(const Holder &) = default;
  Holder(Holder &&) noexcept = default;
  Holder &operator=(Holder &&) noexcept = default;
  ~Holder() {
    close_policy(hid);
  }
};

} // namespace detail

// Generic HDF5 object
template <herr_t close_policy(hid_t)>
class Handle {
private:
  // The handle to the H5 object
  hid_t id_;
  std::shared_ptr<const detail::Holder<close_policy>> self_ = nullptr;

public:
  explicit Handle(hid_t id, bool manage = true) noexcept : id_(id) {
    if (manage) {
      self_ = std::make_shared<detail::Holder<close_policy>>(id);
    }
  }
  auto getID() const {
    return this->id_;
  }

  // Returns name of file that handle id_ is contained within
  std::string filename() const {
    std::string retval;
    auto fileID = H5Iget_file_id(this->getID());

    size_t nChars = 0;
    char *data = nullptr;
    nChars = H5Fget_name(fileID, data, nChars);
    retval.resize(nChars);
    data = &(*retval.begin()); // data() returns const char*
    H5Fget_name(fileID, data,
                nChars + 1); // +1 because that's what HDF5 says to do
    return retval;
  }

  // Returns path (HDF5 name) to object pointed to by id_
  std::string path() const {
    std::string retval;
    size_t nChars = 0;
    char *data = nullptr;
    nChars = H5Iget_name(this->getID(), data, nChars);
    retval.resize(nChars);
    data = &(*retval.begin()); // data() returns const char*
    H5Iget_name(this->getID(), data,
                nChars + 1); // +1 because that's what HDF5 says to do
    return retval;
  }
};

// Aliases for useful types
using PropertiesHandle = Handle<H5Pclose>;
using AttributeHandle = Handle<H5Aclose>;
using SpaceHandle = Handle<H5Sclose>;

} // namespace f5

#endif
