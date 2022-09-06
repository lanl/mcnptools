//Use a std::function to provide a uniform interface to each of the
//event parsers. 
using EventParserInterface = std::function< PtracEvent(size_t index) >;

template<typename Converter, typename Buffer>
class EventParser
{
  private: 
    Converter m_converter;
    Buffer    m_buffer;

  public:
    EventParser(Converter converter, Buffer buffer):
      m_converter( std::move(converter) ),
      m_buffer( std::move(buffer) )
    {}
  
    PtracEvent operator()(size_t index) 
    {
      return m_converter( m_buffer(index) );
    }
};

template<typename Converter, typename Buffer>
auto MakeEventParser(Converter && converter, Buffer && buffer) -> EventParserInterface
{
  auto event_parser = EventParser< 
      std::decay_t<Converter>, 
      std::decay_t<Buffer> > { 
          std::forward<Converter>(converter),
          std::forward<Buffer>(buffer) 
      };

  return EventParserInterface{ event_parser };
}

template<typename Map>
class MultiEventParser
{
  private:
    Map m_event_parsers;

  public:
    MultiEventParser(Map map):
      m_event_parsers( std::move(map) )
    {}

    template<typename Enum>
    auto operator()(size_t index, Enum type)
    {
      try 
      {
        //Convert the event from the corresponding event array into a PtracEvent
        return m_event_parsers.at(type)( index );
      }
      catch(...)
      {
        throw std::runtime_error("Unable to parse event from event arrays");
      }
    }
};

template<typename Map>
auto MakeMultiEventParser(Map && map) {
  return MultiEventParser< std::decay_t<Map> >( std::forward<Map>(map) );
}

inline auto MakeEventProcessor(f5::Group ptrack_group, size_t cache_size)
{
  //Make a parser that takes an index for a particular event array 
  //and returns the corresponding PtracEvent
  auto make_parser = [ptrack_group, cache_size]
    (std::string name, const auto & event)
  {
    auto type = makeH5Compound( event );
    auto event_enum = detail::EventToMcnptoolsEnum( event );
    auto buffer = MakeDatasetBuffer( ptrack_group, std::move(name), 
                                     cache_size, event, type );
    auto event_converter = MakeEventConverter(event, event_enum);
    return MakeEventParser( std::move(event_converter), std::move(buffer) );
  };

  //Map from event type to parser
  std::unordered_map<PtracEnums::PtracEventType, EventParserInterface> parsers;
  parsers.emplace( PtracEnums::SRC, make_parser("Source", Source{}) );
  parsers.emplace( PtracEnums::SUR, make_parser("SurfaceCrossing", SurfaceCrossing{}) );
  parsers.emplace( PtracEnums::BNK, make_parser("Bank", Bank{}) );
  parsers.emplace( PtracEnums::COL, make_parser("Collision", Collision{}) );
  parsers.emplace( PtracEnums::TER, make_parser("Termination", Termination{}) );
  
  return MakeMultiEventParser( std::move( parsers ) );
}
