#define CATCH_CONFIG_MAIN
#include "mcnptools/McnpTools.hpp"
#include <catch2/catch.hpp>
#include <cmath>

using Catch::Matchers::Contains;
using namespace mcnptools;

TEST_CASE( "Ensure enumerated values are not changing between MCNP and MCNPTools" )
{
  /* There is currently no shared repository for enumerated values between
   * MCNP and MCNPTools.  This unit test simply ensures that MCNP tools is
   * using the same values that MCNP was previously. There is no way to easily
   * inspect the value of these numbers from an HDF5 file.  If a file generated
   * by MCNP has different values for these numbers, then the parser will not 
   * be able to find the event types and will have a hard failure of some form
   */
  REQUIRE( static_cast<int>(PtracEnums::SRC) == 1000 );
  REQUIRE( static_cast<int>(PtracEnums::BNK) == 2000 );
  REQUIRE( static_cast<int>(PtracEnums::SUR) == 3000 );
  REQUIRE( static_cast<int>(PtracEnums::COL) == 4000 );
  REQUIRE( static_cast<int>(PtracEnums::TER) == 5000 );
}

TEST_CASE( "Convert HDF5 event types into MCNPTools ptrac event types")
{
  /* Converters are used to convert the concrete types in the HDF5 file
   * written by MCNP into the uniform interface PtracEventType that MCNPTools
   * provides. Converts are unique to each event type because the different
   * event types contain extra data */
  SECTION( "data shared by all events is properly converted")
  {
    //Using a source event, but any event could be used here
    Source event {};
    event.nps=100;               
    event.node=1;               
    event.particle_type = 1; /* ipt */   
    event.num_collisions_this_branch = 5; 
    event.x = 1.;
    event.y = 2.;
    event.z = 3.;
    event.u = 0.4;
    event.v = 0.5;
    event.w = 0.6;
    event.energy=15.;
    event.weight=2.;
    event.time=0.1;
    event.cell_id = 2; /* icl */
    event.material_id = 3; /* mat(icl) */ 

    auto converter = MakeEventConverter(event, Ptrac::SRC);
    auto ptrack_event = converter(event);

    //PtrackEvent returns a double independent of the type of the value
    REQUIRE( ptrack_event.Get(Ptrac::NPS)  == 100. );
    REQUIRE( ptrack_event.Get(Ptrac::NODE)  == 1. );
    REQUIRE( ptrack_event.Get(Ptrac::PARTICLE)  == 1. );
    REQUIRE( ptrack_event.Get(Ptrac::CELL)  == 2. );
    REQUIRE( ptrack_event.Get(Ptrac::MATERIAL)  == 3. );
    REQUIRE( ptrack_event.Get(Ptrac::COLLISION_NUMBER)  == 5. );
    REQUIRE( ptrack_event.Get(Ptrac::X)  == 1. );
    REQUIRE( ptrack_event.Get(Ptrac::Y)  == 2. );
    REQUIRE( ptrack_event.Get(Ptrac::Z)  == 3. );
    REQUIRE( ptrack_event.Get(Ptrac::U)  == 0.4 );
    REQUIRE( ptrack_event.Get(Ptrac::V)  == 0.5 );
    REQUIRE( ptrack_event.Get(Ptrac::W)  == 0.6 );
    REQUIRE( ptrack_event.Get(Ptrac::ENERGY)  == 15. );
    REQUIRE( ptrack_event.Get(Ptrac::WEIGHT)  == 2. );
    REQUIRE( ptrack_event.Get(Ptrac::TIME)  == 0.1 );
  }

  SECTION( "for a source event")
  {
    Source event {};
    event.source_type = 1;
    event.energy = 20.;

    auto converter = MakeEventConverter(event, Ptrac::SRC);
    auto ptrack_event = converter(event);

    REQUIRE( ptrack_event.Get(Ptrac::ENERGY) == 20. );
    REQUIRE( ptrack_event.Type() == Ptrac::SRC );
  }

  SECTION( "for a surface crossing event" )
  {
    SurfaceCrossing event {};
    event.energy = 20.;
    event.surface_normal_cosine = std::sqrt(0.5);
    event.surface_id = 100.1;

    auto converter = MakeEventConverter(event, Ptrac::SUR);
    auto ptrack_event = converter(event);

    REQUIRE( ptrack_event.Get(Ptrac::ENERGY) == 20. );
    REQUIRE( ptrack_event.Get(Ptrac::ANGLE) == Approx(45.0 /*degrees*/) );
    REQUIRE( ptrack_event.Get(Ptrac::SURFACE) == 100.1 );
    REQUIRE( ptrack_event.Type() == Ptrac::SUR );
  }

  SECTION( "for a bank event")
  { 
    //Currently, bank_types are just ints in HDF5 file, but they should
    //be replaced as an enum eventually
    Bank event {};
    event.energy = 20.;
    event.bank_type = Ptrac::BNK_N_XN_F;
    event.zaid = 2004;
    event.reaction_type = 6; //MT number

    auto converter = MakeEventConverter(event, Ptrac::BNK);
    auto ptrack_event = converter(event);

    REQUIRE( ptrack_event.Get(Ptrac::ENERGY) == 20. );
    REQUIRE( ptrack_event.Get(Ptrac::ZAID) == 2004. );
    REQUIRE( ptrack_event.Get(Ptrac::RXN) == 6. );
    CHECK( Ptrac::BNK_N_XN_F == 7 );
    REQUIRE( ptrack_event.Get(Ptrac::BANK_TYPE) == Approx( static_cast<double>(7) ) );
    REQUIRE( ptrack_event.Type() == Ptrac::BNK );

    //Check that interface of getting bank type out works
    REQUIRE( ptrack_event.BankType() == Ptrac::BNK_N_XN_F );
  }

  SECTION( "for a collision" )
  {
    Collision event {};
    event.energy = 20.;
    event.zaid = 2004;
    event.reaction_type = 6; //MT number

    auto converter = MakeEventConverter(event, Ptrac::COL);
    auto ptrack_event = converter(event);

    REQUIRE( ptrack_event.Get(Ptrac::ENERGY) == 20. );
    REQUIRE( ptrack_event.Get(Ptrac::ZAID) == 2004. );
    REQUIRE( ptrack_event.Get(Ptrac::RXN) == 6. );
    REQUIRE( ptrack_event.Type() == Ptrac::COL );
  }

  SECTION( "for a termination event" )
  {
    Termination event {};
    event.energy = 20.;

    //Currently, termination_types are just ints in HDF5 file, but they should
    //be replaced as an enum eventually
    event.termination_type = static_cast<int>(Ptrac::TER_N_FISSION);

    auto converter = MakeEventConverter(event, Ptrac::TER);
    auto ptrack_event = converter(event);

    REQUIRE( ptrack_event.Get(Ptrac::ENERGY) == 20. );
    CHECK( static_cast<int>(Ptrac::TER_N_FISSION) == 14 );
    REQUIRE( ptrack_event.Get(Ptrac::TERMINATION_TYPE) == static_cast<double>(14.));
    REQUIRE( ptrack_event.Type() == Ptrac::TER );
  }
}
