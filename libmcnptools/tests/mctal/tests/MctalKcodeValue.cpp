#include "MctalKcodeFixture.hpp"

TEST_F(MctalKcodeFixture, MctalKcodeValue) {
  double keff = kcode.GetValue(mcnptools::MctalKcode::AVG_COMBINED_KEFF);
  EXPECT_EQ(keff,7.63582E-01);

  double kstd = kcode.GetValue(mcnptools::MctalKcode::AVG_COMBINED_KEFF_STD);
  EXPECT_EQ(kstd,1.72487E-03);
}
