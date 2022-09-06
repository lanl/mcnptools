#include "MeshtalFixture.hpp"

TEST_F(MeshtalFixture, MeshtalNps) {
  int nps = meshtal.GetNps();

  EXPECT_EQ(nps,225000);
}
