#ifndef SHACL_F5_H5UTILITIES_HPP
#define SHACL_F5_H5UTILITIES_HPP

#include "shacl/f5/hdf5_nowarnings.hpp"
#include <stdexcept>
#include <string>

namespace f5 {

inline bool checkH5Link(hid_t id, const std::string &name) {
  return (H5Lexists(id, name.c_str(), H5P_DEFAULT) > 0);
}

inline void deleteH5Link(hid_t id, const std::string &name) {
  if (!checkH5Link(id, name)) {
    throw std::runtime_error(
        "There was a problem attempting to delete link to \"" + name + "\".");
  }
  H5Ldelete(id, name.c_str(), H5P_DEFAULT);
}

inline void createH5Link(hid_t new_parent_id, const std::string &new_name,
                         hid_t old_parent_id, const std::string &old_name) {
  auto err = H5Lcreate_hard(old_parent_id, old_name.c_str(), new_parent_id,
                            new_name.c_str(), H5P_DEFAULT, H5P_DEFAULT);

  if (err != 0) {
    throw std::runtime_error("Unable to create link to \"" + old_name + "\".");
  }
}

inline std::vector<std::string> listH5Names(hid_t id) {
  hsize_t nobjs;
  herr_t err = H5Gget_num_objs(id, &nobjs);

  if (err != 0) {
    throw std::runtime_error("Problem asking for number of objects in Group");
  }

  std::vector<std::string> mylist;
  for (hsize_t i = 0; i < nobjs; ++i) {
    // C function needs C NULL ptr, not C++ nullptr
    auto ssize = H5Lget_name_by_idx(id, ".", H5_INDEX_NAME, H5_ITER_INC, i,
                                    NULL, 0, H5P_DEFAULT) +
                 1; // NOLINT
    if (ssize < 1) {
      throw std::runtime_error(
          "Problem asking for objname length of object idx: " +
          std::to_string(i));
    }

    std::vector<char> objname(ssize);
    auto ssize2 = H5Lget_name_by_idx(id, ".", H5_INDEX_NAME, H5_ITER_INC, i,
                                     objname.data(), ssize, H5P_DEFAULT);
    if (ssize2 < 0) {
      throw std::runtime_error("Problem asking for objname of object idx: " +
                               std::to_string(i));
    }
    std::string strname(objname.data());
    mylist.emplace_back(std::move(strname));
  }
  return mylist;
}

} // namespace f5

#endif
