#include "MctalFixture.hpp"

TEST_F(MctalFixture, MctalComment) {
  std::string comment = mctal.GetComment();

  EXPECT_EQ(comment," Input file to generate Mctal, Meshtal, and Ptrac files for MCNPTools testing   ");
}
