#include "gtest/gtest.h"
#include "mcnptools/McnpTools.hpp"

class MctalFixture : public ::testing::Test {
public:
  mcnptools::Mctal mctal;
  std::vector<int> tallies;

  MctalFixture() : mctal("test_mctal") {
    tallies = mctal.GetTallyList();
  };

};

class MctalFixture2 : public ::testing::Test {
public:
  mcnptools::Mctal mctal_prdmp_neg_1;
  std::vector<int> tallies_prdmp_neg_1;

  MctalFixture2() : mctal_prdmp_neg_1("test_mctal_prdmp_neg_1") {
    tallies_prdmp_neg_1 = mctal_prdmp_neg_1.GetTallyList();
  };

};


