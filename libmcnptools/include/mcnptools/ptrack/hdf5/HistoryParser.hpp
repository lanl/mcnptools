namespace detail {
    
  template<typename RecordParser, typename EventProcessor>
  auto ReadNextHistoryEvents(RecordParser & record_log, EventProcessor & event_processor, size_t start)
  {
    //Read all the records that have the history associated with the first
    //event processed.  All events for a history are contiguous in the
    //record log
    std::vector<PtracEvent> events;
    auto first_event = record_log(start);
    auto log_size = record_log.size();
    auto read_history = first_event.nps;
    while( true )
    {
      auto event_record = record_log(start);
      if( event_record.nps != read_history )
      {
        break;
      }
      else
      {
        auto event_type = event_record.type;
        auto array_index = event_record.event_array_index;

        auto new_event = event_processor(array_index, event_type);
        //Check event NPS is valid incase file has been corrupted
        if (new_event.Get( PtracEnums::NPS ) != read_history) {
          throw std::runtime_error("NPS of event " + std::to_string(event_type) 
             + " at index " + std::to_string(array_index) 
             + " did not match NPS in record log");
        }
        events.push_back( std::move(new_event) );

        start++;
        if ( start == log_size ) {
          break; //Last available entry has been read
        } 
      }
    }
    return std::make_pair(read_history, std::move(events));
  }
}
 
template<typename RecordBuffer, typename EventProcessor>
class HDF5PtrackParser
{
  protected:
    RecordBuffer m_record_buffer;
    EventProcessor m_event_parser;
    f5::File m_file;
    unsigned int next_record  = 0;

    void ReadMoreHistories(unsigned int read_num, std::vector<PtracHistory> & histories)
    {
      using NPSType = std::decay_t<decltype(EventRecord{}.nps)>;

      for(unsigned int i=0; i < read_num; ++i) {
        if (EndOfFile()) {
          break;
        }
        //Read out nps number and events
        std::vector<PtracEvent> events;
        NPSType nps;
        std::tie(nps, events) = detail::ReadNextHistoryEvents(m_record_buffer, m_event_parser, next_record);

        next_record += events.size();
        PtracNps nps_data( nps ); //TODO add filter information once it is written to HDF5 file
        histories.emplace_back(nps_data, std::move(events) );
      }
    }

  public:
    HDF5PtrackParser(f5::File file, RecordBuffer record_buffer,  EventProcessor event_parser):
      m_file( std::move(file) ),
      m_record_buffer( std::move(record_buffer) ),
      m_event_parser( std::move(event_parser) )
    {} 

    bool EndOfFile() const 
    {
      return next_record >= m_record_buffer.size();
    }

    std::vector<PtracHistory> ReadHistories(const unsigned int& num_hists)
    {
      std::vector<PtracHistory> histories;
      histories.reserve(num_hists);

      ReadMoreHistories(num_hists, histories);

      return histories;
    }
};

inline auto MakeHDF5PtrackParser(f5::File file, std::string group_name = "ptrack")
{
  constexpr const auto record_log_cache_size = 1000;
  constexpr const auto event_array_cache_size = 200;

  auto group = file.openGroup( group_name );


  auto event_record = EventRecord{};
  auto record_buffer = MakeDatasetBuffer( group, "RecordLog", record_log_cache_size,
      EventRecord{}, makeH5Compound(EventRecord{}) );

  auto event_processor = MakeEventProcessor( group, event_array_cache_size );
  return HDF5PtrackParser< decltype(record_buffer), decltype(event_processor) >(
      std::move(file), std::move(record_buffer), std::move(event_processor) );
}
