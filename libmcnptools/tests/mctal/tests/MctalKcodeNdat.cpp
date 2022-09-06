#include "MctalKcodeFixture.hpp"

TEST_F(MctalKcodeFixture, MctalKcodeNdat) {
  int ndat = kcode.GetNdat();

  EXPECT_EQ(ndat,19);
}
