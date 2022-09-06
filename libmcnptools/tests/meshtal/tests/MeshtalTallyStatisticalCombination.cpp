#include <cmath>
#include "MeshtalTallyFixture.hpp"


TEST_F(MeshtalTallyFixture, MeshtalTallyStatisticalCombination) {
  mcnptools::MeshtalTally tally = tally_14;
  int nps = meshtal.GetNps();

  double value = tally_14.GetValue(2,2,2);
  double error = tally_14.GetError(2,2,2);

  double m1 = value * nps;
  double m2 = nps * std::pow(value,2)*(std::pow(error,2)*(nps-1)+1);

  tally &= tally_14;

  double value_new = (2*m1)/(2*nps);
  double error_new = std::sqrt( ((2*m2)/(2*nps) - std::pow(value_new,2))/(2*nps-1) )/value_new;

  EXPECT_NEAR(value, value_new, 1e-6);
  EXPECT_NEAR(error/std::sqrt(2.0), error_new, 1e-6);
}
