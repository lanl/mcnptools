#include "MctalFixture.hpp"

TEST_F(MctalFixture, MctalCode) {
  std::string code = mctal.GetCode();

  EXPECT_EQ(code,"mcnp");
}
