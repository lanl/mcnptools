#include "BinPtracFixture.hpp"

TEST_F(BinPtracFixture, BinPtracHistoryNumEvents) {
  std::vector<mcnptools::PtracHistory> hists = ptrac.ReadHistories(1);

  EXPECT_EQ(hists.at(0).GetNumEvents(),14);
}
