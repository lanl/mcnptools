#include "gtest/gtest.h"
#include "mcnptools/McnpTools.hpp"

class MctalTallyFixture : public ::testing::Test {
public:
  mcnptools::Mctal mctal;
  mcnptools::MctalTally tally_01;
  mcnptools::MctalTally tally_04;

  MctalTallyFixture() : mctal("test_mctal") {
    tally_01 = mctal.GetTally(1);
    tally_04 = mctal.GetTally(4);
  };
};
