#include "MctalTallyFixture.hpp"

TEST_F(MctalTallyFixture, MctalTallyFBins) {
  // tally 1
  std::vector<double> check = { 101, 102 };
  std::vector<double> fbins = tally_01.GetFBins();

  ASSERT_EQ(fbins.size(),2);

  for(size_t i=0; i<check.size(); ++i)
    EXPECT_EQ(fbins[i],check[i]);

  // tally 4
  check = { 1001, 1002};
  fbins = tally_04.GetFBins();

  ASSERT_EQ(fbins.size(),2);

  for(size_t i=0; i<check.size(); ++i)
    EXPECT_EQ(fbins[i],check[i]);
}
