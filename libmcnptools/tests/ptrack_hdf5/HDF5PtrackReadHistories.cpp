#define CATCH_CONFIG_MAIN
#include "mcnptools/McnpTools.hpp"
#include "shacl/f5.hpp"
#include <catch2/catch.hpp>
#include <stdio.h>
#include <type_traits>
#include <numeric>

using Catch::Matchers::Contains;
using namespace mcnptools;

TEST_CASE( "Read histories from an HDF5 file" )
{
  auto file = f5::File("tests/parallel_testing.h5", 'r');
  auto history_parser = MakeHDF5PtrackParser(file, "ptrack");

  auto histories = history_parser.ReadHistories(100);

  SECTION( "Do the first two histories in the file match, despite not having ordered NPS" )
  {
    //This ptrack file was generated in parallel.  Thus, this
    //test is likely to fail if you regenerate this file.
    auto event1_second = histories[0].GetEvent(1);
    auto event2_first = histories[1].GetEvent(0);

    REQUIRE(event1_second.Type() == Ptrac::SUR);
    REQUIRE(event2_first.Type() == Ptrac::SRC);

    CHECK(event1_second.Get(Ptrac::X) == Approx(0.4612549012) );
    CHECK(event2_first.Get(Ptrac::X) == Approx(0.590088005) );
  }

  SECTION( "Read again to get the last 100 histories" )
  {
    histories = history_parser.ReadHistories(100);

    //Histories are not sorted, so we need to find the history that satisfies
    //our desired NPS, in this case 191
    auto history_191 = *std::find_if(histories.begin(), histories.end(), 
                                 [](const auto & history) {
                                      return history.GetNPS().NPS() == 191;
                                 });
    
    auto number_events = history_191.GetNumEvents();
    auto last_nps_191 =  history_191.GetEvent(number_events - 1);
    auto first_nps_191 = history_191.GetEvent(0);
    REQUIRE(first_nps_191.Type() == Ptrac::SRC);
    REQUIRE(first_nps_191.Get(Ptrac::X) == Approx(-0.3824981839) );
    REQUIRE(last_nps_191.Type() == Ptrac::SUR);
    REQUIRE(last_nps_191.Get(Ptrac::X) == Approx(-0.5162597346) );
  }

  SECTION( "Reading more histories than available in the file should just return a truncated list")
  {
    histories = history_parser.ReadHistories(500);
    REQUIRE(history_parser.EndOfFile());
    REQUIRE(histories.size() == 100);
    
    auto empty_vector = history_parser.ReadHistories(10);
    REQUIRE(empty_vector.size() == 0);
  }
}

TEST_CASE( "If I read the whole file and sort, all NPS are there when no filters were used")
{
  std::vector<unsigned int> desired_history_numbers(200);
  std::iota(desired_history_numbers.begin(), desired_history_numbers.end(), 1);

  auto file = f5::File("tests/parallel_testing.h5", 'r');
  auto history_parser = MakeHDF5PtrackParser(file, "ptrack");

  auto histories = history_parser.ReadHistories(1000);
  REQUIRE(histories.size() == 200);

  //Sort the histories by NPS number.  Because of move semantics, this is relatively
  //cheap
  std::sort(histories.begin(), histories.end(), [](auto hist1, auto hist2) {
      return hist1.GetNPS().NPS() < hist2.GetNPS().NPS();
     });

  for(int i=0; i < 200; ++i ) {
    CHECK( histories[i].GetNPS().NPS() == desired_history_numbers[i] );
  }
}

TEST_CASE( "Read histories using the Ptrac interface" )
{
  auto ptrac = Ptrac("tests/parallel_testing.h5", Ptrac::HDF5_PTRAC);
  auto histories = ptrac.ReadHistories(500);

  //Histories are not sorted, so we need to find the history that satisfies
  //our desired NPS, in this case 191
  auto history_191 = *std::find_if(histories.begin(), histories.end(), 
                               [](const auto & history) {
                                    return history.GetNPS().NPS() == 191;
                               });

  //Check that a parituclar history matches expected result from file
  auto number_events = history_191.GetNumEvents();
  auto last_nps_191 =  history_191.GetEvent(number_events - 1);
  auto first_nps_191 = history_191.GetEvent(0);
  REQUIRE(first_nps_191.Type() == Ptrac::SRC);
  REQUIRE(first_nps_191.Get(Ptrac::X) == Approx(-0.3824981839) );
  REQUIRE(last_nps_191.Type() == Ptrac::SUR);
  REQUIRE(last_nps_191.Get(Ptrac::X) == Approx(-0.5162597346) );
}

TEST_CASE( "Read a serial file containing all event types and make sure processing is correct" )
{
  auto ptrac = Ptrac("tests/ptrack_all_events.h5", Ptrac::HDF5_PTRAC);
  auto histories = ptrac.ReadHistories(10);

  for(size_t i =1; i <= histories.size(); ++i) {
    REQUIRE(histories[i-1].GetNPS().NPS() == i );
  }

  //Check various events in the file for correctness
  auto src_event = histories[9].GetEvent(0);  
  REQUIRE( src_event.Get(Ptrac::ENERGY) == 14.1 );
  REQUIRE( src_event.Get(Ptrac::Z) == 0.0 );
  REQUIRE( src_event.Get(Ptrac::U) == Approx(0.194583) );

  auto sur_event = histories[0].GetEvent(2);
  REQUIRE( sur_event.Get(Ptrac::ENERGY) == 14.1 );
  REQUIRE( sur_event.Get(Ptrac::ANGLE) == Approx(0.0 /*degrees*/) );
  REQUIRE( sur_event.Get(Ptrac::SURFACE) == 131. );
  REQUIRE( sur_event.Type() == Ptrac::SUR );

  auto col_event = histories[3].GetEvent(12);
  REQUIRE( col_event.Get(Ptrac::X) == Approx(2.00723) );
  REQUIRE( col_event.Get(Ptrac::ENERGY) == Approx(1.70539) );
  REQUIRE( col_event.Get(Ptrac::ZAID) == 94239. );
  REQUIRE( col_event.Get(Ptrac::RXN) == 2. );
  REQUIRE( col_event.Type() == Ptrac::COL );

  auto ter_event = histories[1].GetEvent( histories[1].GetNumEvents() - 1 );
  REQUIRE( ter_event.Get(Ptrac::ENERGY) == Approx(14.1) );
  CHECK( static_cast<int>(Ptrac::TER_ESCAPE) == 1);
  REQUIRE( ter_event.Get(Ptrac::TERMINATION_TYPE) == static_cast<double>(1.));
  REQUIRE( ter_event.Type() == Ptrac::TER );

  auto bnk_event = histories[3].GetEvent(25);
  REQUIRE( bnk_event.Type() == Ptrac::BNK );
  REQUIRE( bnk_event.Get(Ptrac::Z) == Approx(-2.37668) );
  REQUIRE( bnk_event.Get(Ptrac::ZAID) == 94239. );
  REQUIRE( bnk_event.Get(Ptrac::RXN) == 19 );
  CHECK( Ptrac::BNK_N_XN_F == 7 );
  REQUIRE( bnk_event.BankType() == Ptrac::BNK_N_XN_F );
}
