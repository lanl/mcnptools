template<typename EventProcessor, typename RecordBuffer>
class EventRecordParser
{
  private:
    EventProcessor m_event_processor;
    RecordBuffer   m_record_buffer;

  public:

    EventRecordParser(EventProcessor parsers_map, 
                      RecordBuffer  record_buffer):
      m_event_processor( std::move(parsers_map) ),
      m_record_buffer( std::move(record_buffer) )
    {}

    PtracEvent operator()(size_t index) 
    {
      auto event_record = m_record_buffer( index );
      auto event_type = event_record.type;
      auto array_index = event_record.event_array_index;

      return m_event_processor(array_index, event_type);
    }
};

inline auto MakeEventRecordParser(f5::Group ptrack_grp, 
                                  std::string record_name,
                                  size_t cache_size = 500)
{
  auto event_handler = MakeEventProcessor(ptrack_grp, cache_size);
  
  EventRecord event_record {};
  auto record_buffer = MakeDatasetBuffer( ptrack_grp, record_name, cache_size, 
                                          event_record, makeH5Compound(event_record) );

  return EventRecordParser<decltype(event_handler),decltype(record_buffer)> (
            std::move(event_handler), std::move(record_buffer) );
}
