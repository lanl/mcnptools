#include "BinPtracFixture.hpp"

TEST_F(BinPtracFixture, BinPtracHistoryNps) {
  std::vector<mcnptools::PtracHistory> hists = ptrac.ReadHistories(34);

  int64_t nps = hists.at(33).GetNPS().NPS();

  EXPECT_EQ(nps,34);
}
