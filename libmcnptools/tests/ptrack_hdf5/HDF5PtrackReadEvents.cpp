#define CATCH_CONFIG_MAIN
#include "mcnptools/McnpTools.hpp"
#include "shacl/f5.hpp"
#include <catch2/catch.hpp>
#include <stdio.h>
#include <type_traits>

using Catch::Matchers::Contains;
using namespace mcnptools;


TEST_CASE( "Events are read correctly from a file" )
{
  auto file = f5::File("tests/testing.h5", 'r');
  auto group = file.openGroup("ptrack");

  SECTION( "Read a single type of event from a buffer" )
  {
    //Mock out a buffer with one that returns a source event
    //with the passed in index + 1 defined as nps
    struct MockBuffer {
      auto operator()(size_t index) {
        Source src;
        src.nps = index + 1;
        return src;
      }
    };
    auto buffer = MockBuffer{};
    auto converter = MakeEventConverter(Source{}, Ptrac::SRC);

    auto event_parser = MakeEventParser(converter, std::move(buffer) );
    auto event = event_parser(10);

    REQUIRE( std::is_same<decltype(event), PtracEvent>::value );
    REQUIRE( event.Get(Ptrac::NPS) == 11 );
    REQUIRE( event.Has(Ptrac::SOURCE_TYPE) );
  }

  SECTION( "Using an event handler to process from individual arrays" )
  {
    auto processor = MakeEventProcessor(group, 2);

    //Processor takes in enums defined in MCNPTools whose
    //values must match those in the HDF5 files for types
    std::vector<PtracEvent> events;
    events.emplace_back( processor(0, PtracEnums::SRC) );
    events.emplace_back( processor(1, PtracEnums::SUR) );
   
    REQUIRE(events[0].Type() == Ptrac::SRC);
    REQUIRE(events[1].Type() == Ptrac::SUR);

    REQUIRE(events[0].Get(Ptrac::X) == Approx(0.471468) );
    REQUIRE(events[1].Get(Ptrac::Z) == Approx(0.457) );
  }

  SECTION("Processing single events from the event record log")
  {
    auto parser = MakeEventRecordParser(group,"RecordLog", 100);   

    //Parse the first four events from a file and make sure they match
    //expected results
    std::vector<PtracEvent> events;
    events.emplace_back( parser(0) );
    events.emplace_back( parser(1) );
    events.emplace_back( parser(2) );
    events.emplace_back( parser(3) );

    REQUIRE(events[0].Type() == Ptrac::SRC);
    REQUIRE(events[1].Type() == Ptrac::SUR);
    REQUIRE(events[2].Type() == Ptrac::SUR);
    REQUIRE(events[3].Type() == Ptrac::SRC);

    REQUIRE(events[0].Get(Ptrac::X) == Approx(0.471468) );
    REQUIRE(events[0].Has(Ptrac::SOURCE_TYPE) );
    REQUIRE(events[1].Get(Ptrac::X) == Approx(0.461255) );
    REQUIRE(events[2].Get(Ptrac::X) == Approx(0.467209) );
    REQUIRE(events[2].Has(Ptrac::ANGLE) );
    REQUIRE(events[3].Get(Ptrac::X) == Approx(0.207931) );
  }
}
