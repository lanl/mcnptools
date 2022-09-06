#include "MctalFixture.hpp"

TEST_F(MctalFixture, MctalRandoms) {
  long int randoms = mctal.GetRandoms();

  EXPECT_EQ(randoms,62983502);
}
