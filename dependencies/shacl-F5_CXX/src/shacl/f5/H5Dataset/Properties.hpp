#ifdef PHDF5
namespace f5 {

inline auto collectiveProperty() {
  auto dataset_plist_id = H5Pcreate(H5P_DATASET_XFER);
  H5Pset_dxpl_mpio(dataset_plist_id, H5FD_MPIO_COLLECTIVE);
  return PropertiesHandle{dataset_plist_id};
}

} // namespace f5

#endif // PHDF5
