#include "MeshtalTallyFixture.hpp"

TEST_F(MeshtalTallyFixture, MeshtalTallyVolume) {
  EXPECT_NEAR(tally_14.GetVolume(2,2,2), tally_44.GetVolume(2,2,2), 1e-6);
  EXPECT_NEAR(tally_24.GetVolume(2,2,2), tally_44.GetVolume(2,2,2), 1e-6);
  EXPECT_NEAR(tally_34.GetVolume(2,2,2), tally_44.GetVolume(2,2,2), 1e-6);

  EXPECT_NEAR(tally_54.GetVolume(1,1,1), tally_84.GetVolume(1,1,1), 5e-5);
  EXPECT_NEAR(tally_64.GetVolume(1,1,1), tally_84.GetVolume(1,1,1), 5e-5);
  EXPECT_NEAR(tally_74.GetVolume(1,1,1), tally_84.GetVolume(1,1,1), 5e-5);
}
