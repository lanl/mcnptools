#ifndef SHACL_F5_H5FILE_HPP
#define SHACL_F5_H5FILE_HPP

#include <fstream>
#include <memory>
#include <stdexcept>

#include "shacl/f5/H5Attribute.hpp"
#include "shacl/f5/H5Dataset.hpp"
#include "shacl/f5/H5Group.hpp"
#include "shacl/f5/H5Holders.hpp"
#include "shacl/f5/H5Utilities.hpp"

#include "shacl/f5/hdf5_nowarnings.hpp"

namespace f5 {

// Library always creates files using V110 featureset, and does not use V112.
constexpr const auto DEFAULT_LIBVER = H5F_LIBVER_V110;

inline bool checkFile(const std::string &file_name) {
  // Ensure file is accessible
  std::ifstream file(file_name, std::fstream::in);
  return static_cast<bool>(file);
}

namespace detail {
  inline PropertiesHandle creationProperties() {
    // Use FS_PERSIST during file creation, so free space from object deletion is
    // tracked between file opens. Otherwise, deleting an object and closing a
    // file will result in the free space being lost.
    // Remaining options are HDF5 1.10 defaults.
    auto creation_handle = H5Pcreate(H5P_FILE_CREATE);
    H5Pset_file_space_strategy(creation_handle, H5F_FSPACE_STRATEGY_FSM_AGGR,
                               static_cast<hbool_t>(true),
                               static_cast<hsize_t>(1));

    PropertiesHandle creation_properties{creation_handle};
    return creation_properties;
  }
}


inline hid_t createFile(const std::string &file_name, const char mode,
                 const PropertiesHandle &plist_id) {
  hid_t file;
  auto c_prop = detail::creationProperties();

  switch (mode) {
  case 'r':
    file = H5Fopen(file_name.c_str(), H5F_ACC_RDONLY, plist_id.getID());
    break;
  case 'w':
    file = H5Fcreate(file_name.c_str(), H5F_ACC_TRUNC, c_prop.getID(),
                     plist_id.getID());
    break;
  case 'a':
    if (checkFile(file_name)) {
      file = H5Fopen(file_name.c_str(), H5F_ACC_RDWR, plist_id.getID());
    } else {
      file = H5Fcreate(file_name.c_str(), H5F_ACC_TRUNC, c_prop.getID(),
                       plist_id.getID());
    }
    break;
  default:
    throw std::invalid_argument("HDF5 file open mode is invalid.");
  }

  if (file < 0) {
    throw std::runtime_error("Unable to access HDF5 file.");
  }

  return file;
}

inline hid_t createFile(const std::string &file_name, const char mode) {
  auto plist_id = H5Pcreate(H5P_FILE_ACCESS);
  H5Pset_libver_bounds(plist_id, H5F_LIBVER_EARLIEST, DEFAULT_LIBVER);
  PropertiesHandle properties{plist_id};

  if (properties.getID() < 0) {
    throw std::runtime_error("Unable to create HDF5 parameters.");
  }
  return createFile(file_name, mode, properties);
}

#ifdef PHDF5
inline hid_t createFile(const std::string &file_name, char mode, const MPI_Comm &comm,
                 const MPI_Info &info) {
  auto plist_id = H5Pcreate(H5P_FILE_ACCESS);
  H5Pset_fapl_mpio(plist_id, comm, info);
  H5Pset_libver_bounds(plist_id, H5F_LIBVER_EARLIEST, DEFAULT_LIBVER);
  PropertiesHandle properties{plist_id};

  if (properties.getID() < 0) {
    throw std::runtime_error("Unable to create parallel HDF5 parameters.");
  }

  return createFile(file_name, mode, properties);
}
#endif // PHDF5


class File : public Handle<H5Fclose> {
public:
  explicit File(hid_t id) : Handle(id) {}

  File(const std::string &file_name, const char mode)
      : File(createFile(file_name, mode)) {}

#ifdef PHDF5
  File(const std::string &file_name, const char mode, const MPI_Comm &comm,
       MPI_Info info = MPI_INFO_NULL)
      : File(createFile(file_name, mode, comm, info)) {}
#endif // PHDF5

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
