#include "MctalFixture.hpp"

TEST_F(MctalFixture, MctalNps) {
  long int nps = mctal.GetNps();
  EXPECT_EQ(nps,248943);
}

TEST_F(MctalFixture2, MctalNps2) {
  long int nps_prdmp_neg_1 = mctal_prdmp_neg_1.GetNps();
  EXPECT_EQ(nps_prdmp_neg_1, 244023);
}
