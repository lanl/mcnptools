#include "MeshtalTallyFixture.hpp"

TEST_F(MeshtalTallyFixture, MeshtalTallyError) {
  // tally 14
  double error = tally_14.GetError(1,1,1);
  EXPECT_EQ(error, 1.01983E-02);

  // tally 24
  error = tally_24.GetError(1,1,1); // total e bin
  EXPECT_EQ(error, 1.01983E-02);
  error = tally_24.GetError(1,1,1,3); // total e bin
  EXPECT_EQ(error, 1.01983E-02);

  error = tally_24.GetError(1,1,1,1);
  EXPECT_EQ(error, 1.91767E-02);

  // tally 34
  error = tally_34.GetError(1,1,1);
  EXPECT_EQ(error, 1.01749E-02); // total t bin
  error = tally_34.GetError(1,1,1,0,3);
  EXPECT_EQ(error, 1.01749E-02); // total t bin

  error = tally_34.GetError(1,1,1,0,1);
  EXPECT_EQ(error, 1.02654E-02);

  // tally 44
  error = tally_44.GetError(1,1,1);
  EXPECT_EQ(error, 1.01749E-02); // total e&t bin

  error = tally_44.GetError(1,1,1,1);
  EXPECT_EQ(error, 2.26073E-02); // total t bin

  error = tally_44.GetError(1,1,1,-1,1);
  EXPECT_EQ(error, 1.02654E-02); // total e bin

  error = tally_44.GetError(1,1,1,1,1);
  EXPECT_EQ(error, 2.44883E-02); // total e bin
}
