#include "MctalKcodeFixture.hpp"

TEST_F(MctalKcodeFixture, MctalKcodeCycles) {
  int cycles = kcode.GetCycles();
  EXPECT_EQ(cycles,50);

  cycles = kcode.GetSettle();
  EXPECT_EQ(cycles,5);
}
