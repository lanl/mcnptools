#include "MctalTallyFixture.hpp"

TEST_F(MctalTallyFixture, MctalTallyTFC) {
  size_t ntfc = tally_01.GetTFCSize();

  ASSERT_EQ(ntfc,1);

  // check function with default args
  EXPECT_EQ( static_cast<int>(tally_01.GetTFCValue(mcnptools::MctalTally::TFC_NPS)), 248943 );
  EXPECT_EQ( tally_01.GetTFCValue(mcnptools::MctalTally::TFC_VALUE), 8.34542E-01);
  EXPECT_EQ( tally_01.GetTFCValue(mcnptools::MctalTally::TFC_ERROR), 8.29886E-04);
  EXPECT_EQ( tally_01.GetTFCValue(mcnptools::MctalTally::TFC_FOM), 5.09070E+06);

  // check function with specified tfc entry
  EXPECT_EQ( static_cast<int>(tally_01.GetTFCValue(mcnptools::MctalTally::TFC_NPS,0)), 248943 );
  EXPECT_EQ( tally_01.GetTFCValue(mcnptools::MctalTally::TFC_VALUE,0), 8.34542E-01);
  EXPECT_EQ( tally_01.GetTFCValue(mcnptools::MctalTally::TFC_ERROR,0), 8.29886E-04);
  EXPECT_EQ( tally_01.GetTFCValue(mcnptools::MctalTally::TFC_FOM,0), 5.09070E+06);
}
