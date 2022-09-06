#include "gtest/gtest.h"
#include "mcnptools/McnpTools.hpp"

class MeshtalTallyFixture : public ::testing::Test {
public:
  mcnptools::Meshtal meshtal;
  mcnptools::MeshtalTally tally_14;
  mcnptools::MeshtalTally tally_24;
  mcnptools::MeshtalTally tally_34;
  mcnptools::MeshtalTally tally_44;
  mcnptools::MeshtalTally tally_54;
  mcnptools::MeshtalTally tally_64;
  mcnptools::MeshtalTally tally_74;
  mcnptools::MeshtalTally tally_84;

  MeshtalTallyFixture() : meshtal("test_meshtal") {
    tally_14 = meshtal.GetTally(14);
    tally_24 = meshtal.GetTally(24);
    tally_34 = meshtal.GetTally(34);
    tally_44 = meshtal.GetTally(44);
    tally_54 = meshtal.GetTally(54);
    tally_64 = meshtal.GetTally(64);
    tally_74 = meshtal.GetTally(74);
    tally_84 = meshtal.GetTally(84);
  };
};
