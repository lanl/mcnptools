#include "gtest/gtest.h"
#include "mcnptools/McnpTools.hpp"

class MeshtalFixture : public ::testing::Test {
public:
  mcnptools::Meshtal meshtal;
  std::vector<int> tallies;

  MeshtalFixture() : meshtal("test_meshtal") {
    tallies = meshtal.GetTallyList();
  };
};
