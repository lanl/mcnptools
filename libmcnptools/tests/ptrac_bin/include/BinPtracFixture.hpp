#include "gtest/gtest.h"
#include "mcnptools/McnpTools.hpp"

class BinPtracFixture : public ::testing::Test {
public:
  mcnptools::Ptrac ptrac;

  BinPtracFixture() : ptrac("test_ptrac", mcnptools::Ptrac::BIN_PTRAC) {};
};
