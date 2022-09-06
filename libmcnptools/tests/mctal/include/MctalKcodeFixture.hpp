#include "gtest/gtest.h"
#include "mcnptools/McnpTools.hpp"

class MctalKcodeFixture : public ::testing::Test {
public:
  mcnptools::Mctal mctal;
  mcnptools::MctalKcode kcode;

  MctalKcodeFixture() : mctal("test_mctal") {
    kcode = mctal.GetKcode();
  };
};
