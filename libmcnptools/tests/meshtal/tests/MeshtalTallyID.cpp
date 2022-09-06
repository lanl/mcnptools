#include "MeshtalTallyFixture.hpp"

TEST_F(MeshtalTallyFixture, MeshtalTallyID) {
  int id = tally_14.ID();
  EXPECT_EQ(id,14);

  id = tally_54.ID();
  EXPECT_EQ(id,54);
}
