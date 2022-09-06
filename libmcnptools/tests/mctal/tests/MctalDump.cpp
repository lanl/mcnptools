#include "MctalFixture.hpp"

TEST_F(MctalFixture, MctalDump) {
  int dump = mctal.GetDump();

  EXPECT_EQ(dump,2);
}
