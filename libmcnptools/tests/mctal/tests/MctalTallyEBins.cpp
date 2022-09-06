#include "MctalTallyFixture.hpp"

TEST_F(MctalTallyFixture, MctalTallyEBins) {
  std::vector<double> check = { 1.00000E-09,  1.00000E-03,  1.00000E+03 };

  // tally 1
  std::vector<double> ebins = tally_01.GetEBins();

  ASSERT_EQ(ebins.size(),3);

  for(size_t i=0; i<check.size(); ++i)
    EXPECT_EQ(ebins[i],check[i]);

  // tally 4
  ebins = tally_04.GetEBins();

  ASSERT_EQ(ebins.size(),3);

  for(size_t i=0; i<check.size(); ++i)
    EXPECT_EQ(ebins[i],check[i]);

}
