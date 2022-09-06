#define ACCESSOR(variable) \
  [](auto && event) { return std::forward<decltype(event)>(event).variable; }

//Mapping from Ptrac::Get() enums to Event Type members
template<typename Event>
auto DefaultAccessors(const Event &)
{
  using Accessor = std::function<double(const Event &)>;
  return std::unordered_map<PtracEnums::PtracDataType,Accessor>({
     { PtracEnums::NPS, ACCESSOR(nps)},
     { PtracEnums::NODE, ACCESSOR(node) },
     { PtracEnums::PARTICLE , ACCESSOR(particle_type) },
     { PtracEnums::CELL , ACCESSOR(cell_id) },
     { PtracEnums::MATERIAL , ACCESSOR(material_id) },
     { PtracEnums::COLLISION_NUMBER , ACCESSOR(num_collisions_this_branch) },
     { PtracEnums::X , ACCESSOR(x) },
     { PtracEnums::Y , ACCESSOR(y) },
     { PtracEnums::Z , ACCESSOR(z) },
     { PtracEnums::U , ACCESSOR(u) },
     { PtracEnums::V , ACCESSOR(v) },
     { PtracEnums::W , ACCESSOR(w) },
     { PtracEnums::ENERGY , ACCESSOR(energy) },
     { PtracEnums::WEIGHT , ACCESSOR(weight) },
     { PtracEnums::TIME , ACCESSOR(time) } 
   });
}

template<typename Map>
void addExtraAccessors(const Source &, Map & map)
{
  map.emplace( PtracEnums::SOURCE_TYPE, ACCESSOR(source_type) );
}

template<typename Map>
void addExtraAccessors(const SurfaceCrossing &, Map & map)
{
  //For backwards compatibility, convert cosine to angle in degrees
  constexpr const double PI = mcnptools::pi;
  constexpr const double RADS_TO_DEGREES = 180./PI;

  auto get_angle_degrees = [=](const auto & event)
  { 
    return std::acos( event.surface_normal_cosine )*RADS_TO_DEGREES;
  };
  map.emplace( PtracEnums::ANGLE, get_angle_degrees );
  map.emplace( PtracEnums::SURFACE, ACCESSOR(surface_id) );
}

template<typename Map>
void addExtraAccessors(const Bank &, Map & map)
{
  //Store the bank type as a double and then overload the .BankType() method
  //to convert it back to a integer
  auto get_bank_type_as_double = [](const auto & event)->double
  {
    auto bank_type = static_cast<PtracEnums::PtracBankType>(event.bank_type);
    return static_cast<double>(bank_type);
  };
  
  map.emplace( PtracEnums::BANK_TYPE, get_bank_type_as_double );
  map.emplace( PtracEnums::RXN, ACCESSOR(reaction_type) );
  map.emplace( PtracEnums::ZAID, ACCESSOR(zaid) );
}

template<typename Map>
void addExtraAccessors(const Termination &, Map & map)
{
  map.emplace( PtracEnums::TERMINATION_TYPE, ACCESSOR(termination_type) );
}

template<typename Map>
void addExtraAccessors(const Collision &, Map & map)
{
  map.emplace( PtracEnums::RXN, ACCESSOR(reaction_type) );
  map.emplace( PtracEnums::ZAID, ACCESSOR(zaid) );
}

template<typename Event> 
auto makeAccessorMap(Event & event) {
  auto default_accessors = DefaultAccessors(event);
  addExtraAccessors(event, default_accessors );
  return default_accessors;
}

template<typename Event, typename AccessorMap>
class EventConverter
{
  private:
    AccessorMap m_accessors;
    int m_type;

  public:

    EventConverter(AccessorMap&& map, int event_type):
      m_accessors( std::move(map) ),
      m_type( event_type )
    {}
                  
    PtracEvent operator()(const Event & event) const
    {
      std::map<int,double> data;
      for(const auto & accessor_pair : this->m_accessors) {
        auto result = accessor_pair.second( event );
        data.emplace(accessor_pair.first,
                     accessor_pair.second( event ) );
      }
      return {m_type, std::move(data)};
    }
};

template<typename Event>
auto MakeEventConverter(const Event & event, int event_type)
{
  auto accessor_map = makeAccessorMap(event);
  return EventConverter<std::decay_t<Event>, decltype(accessor_map)>{ 
      std::move(accessor_map), event_type };
}

#undef ACCESSOR
