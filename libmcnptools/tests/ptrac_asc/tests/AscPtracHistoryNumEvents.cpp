#include "AscPtracFixture.hpp"

TEST_F(AscPtracFixture, AscPtracHistoryNumEvents) {
  std::vector<mcnptools::PtracHistory> hists = ptrac.ReadHistories(1);

  EXPECT_EQ(hists.at(0).GetNumEvents(),14);
}
