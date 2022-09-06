#include "MeshtalTallyFixture.hpp"

TEST_F(MeshtalTallyFixture, MeshtalTallyValue) {
  // tally 14
  double value = tally_14.GetValue(1,1,1);
  EXPECT_EQ(value, 3.00926E-03);

  // tally 24
  value = tally_24.GetValue(1,1,1); // total e bin
  EXPECT_EQ(value, 3.00926E-03);
  value = tally_24.GetValue(1,1,1,3); // total e bin
  EXPECT_EQ(value, 3.00926E-03);

  value = tally_24.GetValue(1,1,1,1);
  EXPECT_EQ(value, 9.72464E-04);

  // tally 34
  value = tally_34.GetValue(1,1,1);
  EXPECT_EQ(value, 2.24113E-03); // total t bin
  value = tally_34.GetValue(1,1,1,0,3);
  EXPECT_EQ(value, 2.24113E-03); // total t bin

  value = tally_34.GetValue(1,1,1,0,1);
  EXPECT_EQ(value, 2.15530E-03);

  // tally 44
  value = tally_44.GetValue(1,1,1);
  EXPECT_EQ(value, 2.24113E-03); // total e&t bin

  value = tally_44.GetValue(1,1,1,1);
  EXPECT_EQ(value, 2.04732E-04); // total t bin

  value = tally_44.GetValue(1,1,1,-1,1);
  EXPECT_EQ(value, 2.15530E-03); // total e bin

  value = tally_44.GetValue(1,1,1,1,1);
  EXPECT_EQ(value, 1.20977E-04); // total e bin
}
