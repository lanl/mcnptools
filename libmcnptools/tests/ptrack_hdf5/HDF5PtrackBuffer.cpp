#define CATCH_CONFIG_MAIN
#include "mcnptools/McnpTools.hpp"
#include "shacl/f5.hpp"
#include <catch2/catch.hpp>
#include <stdio.h>

using namespace mcnptools;

TEST_CASE( "Access elements from a dataset via buffer" )
{
  std::vector<int> small_data = {1,2,3};
  std::vector<unsigned int> large_data = {1,2,3,4,5,6,7};

  auto file = f5::File("h5buffer_test.h5",'w');
  file.writeDataset("small",small_data);
  file.writeDataset("large",large_data);

  SECTION( "Correctly returns the size of the dataset" )
  {
    auto h5type = f5::getSimpleType<unsigned int>();
    auto buffer = MakeDatasetBuffer(file, "large", 3, large_data.front(), h5type);
    REQUIRE( buffer.size() == 7 );
  }
 
  SECTION( "When the total datasize is smaller than the cache size" )
  {
    auto h5type = f5::getSimpleType<int>();
    auto buffer = MakeDatasetBuffer(file, "small", 5, small_data.front(), h5type);

    REQUIRE( buffer(0) == 1 );
    REQUIRE( buffer(1) == 2 );
    REQUIRE( buffer(2) == 3 );

    SECTION( "Throws if you go over the size" )
    {
      REQUIRE_THROWS( buffer(3) );
    }
  }

  SECTION( "When the total datasize is the same as cache size" )
  {
    auto h5type = f5::getSimpleType<int>();
    auto buffer = MakeDatasetBuffer(file, "small", 3, small_data.front(), h5type);

    REQUIRE( buffer(0) == 1 );
    REQUIRE( buffer(1) == 2 );
    REQUIRE( buffer(2) == 3 );

  }

  SECTION( "When the cache size is smaller than the data size" )
  {
    auto h5type = f5::getSimpleType<unsigned int>();
    auto buffer = MakeDatasetBuffer(file, "large", 3, large_data.front(), h5type);
    for(unsigned int i=0; i < large_data.size(); ++i) {
      REQUIRE( buffer(i) == i+1 );
    }

    REQUIRE_THROWS( buffer(7) );
  }

  //Cleanup file
  std::remove( "h5buffer_test.h5" );
}
