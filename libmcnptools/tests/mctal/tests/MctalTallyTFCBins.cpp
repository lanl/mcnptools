#include "MctalTallyFixture.hpp"

TEST_F(MctalTallyFixture, MctalTallyTFCBins) {
  std::vector<int> tfc = tally_01.GetTFCBins();

  ASSERT_EQ(tfc.size(),8);

  std::vector<int> check = { 0, 0, 0, 0, 0, 1, 3, 3 };

  for(size_t i=0; i<check.size(); ++i)
    EXPECT_EQ(tfc[i],check[i]);
}
