#include "MctalFixture.hpp"

TEST_F(MctalFixture, MctalTallyList) {
  std::vector<int> check = {1,4};

  for(size_t i=0; i<check.size(); ++i)
    EXPECT_EQ(tallies[i],check[i]);
}
