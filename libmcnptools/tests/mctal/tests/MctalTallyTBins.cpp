#include "MctalTallyFixture.hpp"

TEST_F(MctalTallyFixture, MctalTallyTBins) {
  std::vector<double> check = { 0.00000E+00,  1.00000E+01,  2.00000E+01 };

  // tally 1`
  std::vector<double> tbins = tally_01.GetTBins();

  ASSERT_EQ(tbins.size(),3);

  for(size_t i=0; i<check.size(); ++i)
    EXPECT_EQ(tbins[i],check[i]);

  // tally 4
  tbins = tally_04.GetTBins();

  ASSERT_EQ(tbins.size(),3);

  for(size_t i=0; i<check.size(); ++i)
    EXPECT_EQ(tbins[i],check[i]);

}
