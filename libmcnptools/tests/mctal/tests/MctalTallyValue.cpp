#include "MctalTallyFixture.hpp"

TEST_F(MctalTallyFixture, MctalTallyValue) {
  double value = tally_01.GetValue(mcnptools::MctalTally::TFC,
                                   mcnptools::MctalTally::TFC,
                                   mcnptools::MctalTally::TFC,
                                   mcnptools::MctalTally::TFC,
                                   mcnptools::MctalTally::TFC,
                                   mcnptools::MctalTally::TFC,
                                   1,
                                   mcnptools::MctalTally::TFC);

  EXPECT_EQ(value,2.14182E-103);
  value = tally_01.GetValue(mcnptools::MctalTally::TFC,
                            mcnptools::MctalTally::TFC,
                            mcnptools::MctalTally::TFC,
                            mcnptools::MctalTally::TFC,
                            mcnptools::MctalTally::TFC,
                            mcnptools::MctalTally::TFC,
                            2,
                            mcnptools::MctalTally::TFC);

  EXPECT_EQ(value,8.32400E-01);
}
