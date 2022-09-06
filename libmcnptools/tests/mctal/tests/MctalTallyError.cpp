#include "MctalTallyFixture.hpp"

TEST_F(MctalTallyFixture, MctalTallyError) {
  double error = tally_04.GetError(1,
                                   mcnptools::MctalTally::TFC,
                                   mcnptools::MctalTally::TFC,
                                   mcnptools::MctalTally::TFC,
                                   mcnptools::MctalTally::TFC,
                                   mcnptools::MctalTally::TFC,
                                   2,
                                   1);

  EXPECT_EQ(error,0.0012);
}
