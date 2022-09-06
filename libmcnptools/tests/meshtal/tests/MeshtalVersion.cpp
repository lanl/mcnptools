#include "MeshtalFixture.hpp"

TEST_F(MeshtalFixture, MeshtalVersion) {
  std::string version = meshtal.GetVersion();

  EXPECT_EQ(version,"6");
}
