#include "MctalTallyFixture.hpp"

TEST_F(MctalTallyFixture, MctalTallyId) {
  int id = tally_04.ID();

  EXPECT_EQ(id,4);
}
