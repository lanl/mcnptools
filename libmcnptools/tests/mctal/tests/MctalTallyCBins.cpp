#include "MctalTallyFixture.hpp"

TEST_F(MctalTallyFixture, MctalTallyCBins) {
  std::vector<double> check = { 0.00000E+00, 1.00000E+00 };

  // tally 1`
  std::vector<double> cbins = tally_01.GetCBins();

  ASSERT_EQ(cbins.size(),2);

  for(size_t i=0; i<check.size(); ++i)
    EXPECT_EQ(cbins[i],check[i]);

  // tally 4
  cbins = tally_04.GetCBins();

  ASSERT_EQ(cbins.size(),1);
}
