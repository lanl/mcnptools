#include "MeshtalFixture.hpp"

TEST_F(MeshtalFixture, MeshtalCode) {
  std::string code = meshtal.GetCode();

  EXPECT_EQ(code,"mcnp");
}
