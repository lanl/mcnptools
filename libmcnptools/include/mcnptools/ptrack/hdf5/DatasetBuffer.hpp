template<typename Group, typename Data>
class DatasetBuffer
{
  protected:

    size_t m_offset = 0;
    std::vector<Data> m_data_cache = {};

    Group m_group;
    std::string m_dataset_name;
    size_t m_cache_size;
    f5::Type m_h5type;
    size_t m_dset_size;

    void UpdateCache(size_t index)
    {
      //Open dataset and update size in case it has been modified
      auto dataset = m_group.openDataset( m_dataset_name, m_h5type );
      m_dset_size = dataset.shape()[0];

      if ( index >= m_dset_size ) {
        std::string err_msg {"Accessing data past end of dataset "
            + m_dataset_name + " with index " + std::to_string(index) 
            + " > size=" + std::to_string(m_dset_size) };
        throw std::runtime_error(err_msg);
      }

      if (m_cache_size >= m_dset_size) {
        //Read entire dataset into cache
        m_offset = 0;
        m_data_cache.resize(m_dset_size);
        dataset.read(m_data_cache);
      }
      else
      {
        //Make sure read doesn't go past the end of the file
        m_offset = (m_dset_size >= index + m_cache_size) ? index : m_dset_size - m_cache_size;
        dataset.read(m_data_cache, m_offset, m_cache_size);
      }
    }
 
  public:

    DatasetBuffer(Group group, std::string dataset_name, size_t buffer_size, f5::Type h5_type,
                  size_t dataset_size):
      m_group( std::move(group) ),
      m_dataset_name( std::move(dataset_name) ),
      m_cache_size( buffer_size ),
      m_h5type( h5_type ),
      m_dset_size(dataset_size)
    {}

    auto size() const
    {
      return m_dset_size;
    }

    auto operator()(size_t index) 
    {
      if (index < m_offset || index >= m_offset + m_data_cache.size()) {
        this->UpdateCache(index);
      }
      return m_data_cache[index - m_offset];
    }
};

template<typename GroupLike, typename Data>
auto MakeDatasetBuffer(GroupLike && group, std::string dataset_name, 
                       size_t buffer_size, const Data & event, f5::Type h5_type)
{
  auto dataset = group.openDataset( dataset_name );
  auto size = dataset.shape()[0];
  auto a = DatasetBuffer<std::decay_t<GroupLike>, Data>( 
      std::forward<GroupLike>(group), dataset_name, buffer_size , h5_type, size
    );
  return a;
}
