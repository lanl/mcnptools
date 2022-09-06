#ifndef SHACL_F5_H5GROUP_HPP
#define SHACL_F5_H5GROUP_HPP

#include <memory>
#include <stdexcept>

#include "shacl/f5/hdf5_nowarnings.hpp"

#include "shacl/f5/H5Attribute.hpp"
#include "shacl/f5/H5Dataset.hpp"
#include "shacl/f5/H5Holders.hpp"
#include "shacl/f5/H5Utilities.hpp"
#include "shacl/f5/H5Properties.hpp"

namespace f5 {
inline hid_t openH5Group(const std::string &group_name, hid_t handle_id) {
  // Opens or creates the group.
  hid_t handle;
  auto status = H5Lexists(handle_id, group_name.c_str(), H5P_DEFAULT);

  if (status < 0) {
    throw std::runtime_error("Unable to check if HDF5 link exists.");
  } else if (status > 0) {
    handle = H5Gopen(handle_id, group_name.c_str(), H5P_DEFAULT);
  } else {
    handle =
        H5Gcreate(handle_id, group_name.c_str(),
                  detail::utf8LinkProperty().getID(), H5P_DEFAULT, H5P_DEFAULT);
  }

  if (handle < 0) {
    throw std::runtime_error("Unable to create HDF5 group.");
  }

  return handle;
}

class Group : public Handle<H5Gclose> {
public:
  explicit Group(hid_t id) : Handle(id) {}

  explicit Group(const hid_t parent_id, const std::string &group_name)
      : Group(openH5Group(group_name, parent_id)) {}

  Group openGroup(const std::string &group_name) const {
    return Group(this->getID(), group_name);
  }

  template <typename... Args>
  Dataset openDataset(const std::string &s, Args &&... args) const {
    return openH5Dataset(this->getID(), s, std::forward<Args>(args)...);
  }

  template <typename... Args>
  Dataset writeDataset(const std::string &s, Args &&... args) const {
    return writeH5Dataset(this->getID(), s, std::forward<Args>(args)...);
  }

  template <typename... Args>
  Dataset readDataset(const std::string &s, Args &&... args) const {
    return readH5Dataset(this->getID(), s, std::forward<Args>(args)...);
  }

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

  [[deprecated("Use checkLink")]] bool checkObject(const std::string &s) const {
    return checkH5Link(this->getID(), s);
  }

  [[deprecated("Use deleteLink")]] void
  deleteObject(const std::string &s) const {
    deleteH5Link(this->getID(), s);
  }

  bool checkLink(const std::string &s) const {
    return checkH5Link(this->getID(), s);
  }

  void deleteLink(const std::string &s) const {
    deleteH5Link(this->getID(), s);
  }

  template <typename T>
  void createLink(const std::string &new_name, T &old_parent,
                  const std::string &old_name) const {
    createH5Link(this->getID(), new_name, old_parent.getID(), old_name);
  }

  std::vector<std::string> listNames() const {
    return listH5Names(this->getID());
  }
};
} // namespace f5

#endif
