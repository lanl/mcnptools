#define PhaseSpaceData()                 \
    double x=0.;                         \
    double y=0.;                         \
    double z=0.;                         \
    double u=0.;                         \
    double v=0.;                         \
    double w=0.;                         \
    double energy=14.;                   \
    double weight=1.;                    \
    double time=0.;                      \
    int64_t nps=0;                       \
    int node=0;                          \
    int cell_id = -1; /* icl */          \
    int material_id = -1; /* mat(icl) */ \
    int particle_type = -1; /* ipt */    \
    int num_collisions_this_branch = 0; 

#define PhaseSpaceConstructor(p)                                \
    x(p.x),                                                    \
    y(p.y),                                                    \
    z(p.z),                                                    \
    u(p.u),                                                    \
    v(p.v),                                                    \
    w(p.w),                                                    \
    energy(p.energy),                                          \
    weight(p.weight),                                          \
    time(p.time),                                              \
    nps(p.nps),                                                \
    node(p.node),                                              \
    cell_id(p.cell_id),                                        \
    material_id(p.material_id),                                \
    particle_type(p.particle_type),                            \
    num_collisions_this_branch(p.num_collisions_this_branch) 

#define SurfaceCrossingDataMacro()      \
    double surface_id = -1.0;           \
    double surface_normal_cosine = 2.0; 

#define CollisionDataMacro()  \
    int zaid = -1;            \
    int reaction_type = -1000; /* TODO replace with enums */ 

#define BankDataMacro()                                      \
    int zaid = -1;                                           \
    int reaction_type = -1000; /* TODO replace with enums */ \
    int bank_type = -1;                                      

#define SourceDataMacro() \
    int source_type = -1;

#define TerminationDataMacro() \
    int termination_type = -1;

struct Source
{
  PhaseSpaceData()
  SourceDataMacro()
};

struct Bank
{
  PhaseSpaceData()
  BankDataMacro()
};

struct Termination
{
  PhaseSpaceData()
  TerminationDataMacro()
};

struct Collision
{
  PhaseSpaceData()
  CollisionDataMacro()
};

struct SurfaceCrossing
{
  PhaseSpaceData()
  SurfaceCrossingDataMacro()
};

struct EventRecord
{
  int64_t nps = 0;
  int node = 0;
  PtracEnums::PtracEventType type;
  uint64_t event_array_index = 0; 
};


#undef PhaseSpaceData
#undef PhaseSpaceConstructor
#undef SurfaceCrossingDataMacro
#undef CollisionDataMacro
#undef TerminationDataMacro
#undef BankDataMacro
#undef SourceDataMacro

