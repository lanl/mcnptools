#include "MeshtalFixture.hpp"

TEST_F(MeshtalFixture, MeshtalComment) {
  std::string comment = meshtal.GetComment();

  EXPECT_EQ(comment,"Input file to generate Mctal, Meshtal, and Ptrac files for MCNPTools testing");
}
