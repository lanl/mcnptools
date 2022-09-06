inline auto getParticleTypeEnum()
{
  //This is currently not actually interoperated; we just use it as a
  //way to write out the strings for each type.  There is no way currently
  //to get the values of these number from MCNP through interoperation
  static const std::unordered_map<int, const char*> particle_types_ {
    { 1,"NEUTRON"      }, //  SYMBOL:  N
    { 2,"PHOTON"       }, //  SYMBOL:  P
    { 3,"ELECTRON"     }, //  SYMBOL:  E
    { 4,"MU_MINUS"     }, //  SYMBOL:  |
    { 5,"ANEUTRON"     }, //  SYMBOL:  Q
    { 6,"NU_E"         }, //  SYMBOL:  U
    { 7,"NU_M"         }, //  SYMBOL:  V
    { 8,"POSITRON"     }, //  SYMBOL:  F
    { 9,"PROTON"       }, //  SYMBOL:  H
    { 10,"LAMBDA0"     }, //  SYMBOL:  L
    { 11,"SIGMA_PLUS"  }, //  SYMBOL:  +
    { 12,"SIGMA_MINUS" }, //  SYMBOL:  -
    { 13,"XI0"         }, //  SYMBOL:  X
    { 14,"XI_MINUS"    }, //  SYMBOL:  Y
    { 15,"OMEGA_MINUS" }, //  SYMBOL:  O
    { 16,"MU_PLUS"     }, //  SYMBOL:  !
    { 17,"ANU_E"       }, //  SYMBOL:  <
    { 18,"ANU_M"       }, //  SYMBOL:  >
    { 19,"APROTON"     }, //  SYMBOL:  G
    { 20,"PI_PLUS"     }, //  SYMBOL:  /
    { 21,"PI_ZERO"     }, //  SYMBOL:  Z
    { 22,"K_PLUS"      }, //  SYMBOL:  K
    { 23,"K0_SHORT"    }, //  SYMBOL:  %
    { 24,"K0_LONG"     }, //  SYMBOL:  ^
    { 25,"ALAMBDA0"    }, //  SYMBOL:  B
    { 26,"ASIGMA_PLUS" }, //  SYMBOL:  _
    { 27,"ASIGMA_MINUS"}, //  SYMBOL:  ~
    { 28,"AXI0"        }, //  SYMBOL:  C
    { 29,"XI_PLUS"     }, //  SYMBOL:  W
    { 30,"AOMEGA_MINUS"}, //  SYMBOL:  @
    { 31,"DEUTERON"    }, //  SYMBOL:  D
    { 32,"TRITON"      }, //  SYMBOL:  T
    { 33,"HELION"      }, //  SYMBOL:  S
    { 34,"ALPHA"       }, //  SYMBOL:  A
    { 35,"PI_MINUS"    }, //  SYMBOL:  *
    { 36,"K_MINUS"     }, //  SYMBOL:  ?
    { 37,"HEAVY_ION"   }, //  SYMBOL:  #
      // Named constants for supplemental MCNP particle types:
    { -1,   "SIGMA_ZERO "},
    { -2,   "ASIGMA_ZERO"},
    { -3,   "PIDROGEN   "},
  };

  //Create an enum representing particle types
  auto enum_type = H5Tenum_create(H5T_NATIVE_INT);
  for(const auto & pair : particle_types_) {
    H5Tenum_insert(enum_type, pair.second, &pair.first);
  }
  return enum_type;
}

template<typename Event, typename Builder>
void addPhaseSpaceMembers(const Event&, Builder& type_builder)
{ 
  type_builder.addItem("x", &Event::x);
  type_builder.addItem("y", &Event::y);
  type_builder.addItem("z", &Event::z);
  type_builder.addItem("u", &Event::u);
  type_builder.addItem("v", &Event::v);
  type_builder.addItem("w", &Event::w);
  type_builder.addItem("energy",  &Event::energy);
  type_builder.addItem("weight",  &Event::weight);
  type_builder.addItem("time",    &Event::time);
  type_builder.addItem("nps", &Event::nps);
  type_builder.addItem("node", &Event::node);
  type_builder.addItem("material_id", &Event::material_id);
  type_builder.addItem("cell_id", &Event::cell_id);

  const auto ptype_enum = getParticleTypeEnum();
  f5::BasicType basic_f5_type(ptype_enum, true);
  f5::Type f5_ptype_enum_type(basic_f5_type, basic_f5_type);
  type_builder.addItem("particle_type", &Event::particle_type, f5_ptype_enum_type);
  type_builder.addItem("num_collisions_this_branch", &Event::num_collisions_this_branch);
}
  
template<typename Builder>
void addH5Members(const SurfaceCrossing& event, Builder& type_builder)
{ 
  addPhaseSpaceMembers(event, type_builder);
  type_builder.addItem("surface_id", &SurfaceCrossing::surface_id);
  type_builder.addItem("surface_normal_cosine", &SurfaceCrossing::surface_normal_cosine);
}

template<typename Builder>
void addH5Members(const Collision& event, Builder& type_builder)
{ 
  addPhaseSpaceMembers(event, type_builder);
  type_builder.addItem("zaid", &Collision::zaid);
  type_builder.addItem("reaction_type", &Collision::reaction_type);
}

template<typename Builder>
void addH5Members(const Source& event, Builder& type_builder)
{ 
  addPhaseSpaceMembers(event, type_builder);
  type_builder.addItem("source_type", &Source::source_type);
}

template<typename Builder>
void addH5Members(const Termination& event, Builder& type_builder)
{ 
  addPhaseSpaceMembers(event, type_builder);
  type_builder.addItem("termination_type", &Termination::termination_type);
}

template<typename Builder>
void addH5Members(const Bank& event, Builder& type_builder)
{ 
  addPhaseSpaceMembers(event, type_builder);
  type_builder.addItem("zaid", &Bank::zaid);
  type_builder.addItem("reaction_type", &Bank::reaction_type);
  type_builder.addItem("bank_type", &Bank::bank_type);
}

template <typename Event>
auto makeH5Compound(const Event& event)
{
  auto type_builder = f5::makeCompoundTypeBuilder(Event{});
  addH5Members(event, type_builder);
  auto f5_type = type_builder.build();
  return f5_type;
}

inline auto makeH5Compound(const EventRecord&)
{
  //Create an enum representing event types
  auto enum_type = H5Tenum_create(H5T_NATIVE_INT);
  auto enum_value = PtracEnums::COL;
  H5Tenum_insert(enum_type, "collision", &enum_value);
  enum_value = PtracEnums::SUR;
  H5Tenum_insert(enum_type, "surface_crossing", &enum_value);
  enum_value = PtracEnums::SRC;
  H5Tenum_insert(enum_type, "source", &enum_value);
  enum_value = PtracEnums::TER;
  H5Tenum_insert(enum_type, "termination", &enum_value);
  enum_value = PtracEnums::BNK;
  H5Tenum_insert(enum_type, "bank", &enum_value);

  auto type_builder = f5::makeCompoundTypeBuilder(EventRecord{});
  type_builder.addItem("nps", &EventRecord::nps);
  type_builder.addItem("node", &EventRecord::node);
  f5::BasicType basic_f5_type(enum_type, true);
  f5::Type f5_enum_type(basic_f5_type, basic_f5_type);
  type_builder.addItem("type", &EventRecord::type, f5_enum_type);
  type_builder.addItem("event_array_index", &EventRecord::event_array_index);
  auto f5_type = type_builder.build();
  return f5_type;
}
