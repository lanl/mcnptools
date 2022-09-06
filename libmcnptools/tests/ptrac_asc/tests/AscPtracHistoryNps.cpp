#include "AscPtracFixture.hpp"

TEST_F(AscPtracFixture, AscPtracHistoryNps) {
  std::vector<mcnptools::PtracHistory> hists = ptrac.ReadHistories(34);

  int64_t nps = hists.at(33).GetNPS().NPS();

  EXPECT_EQ(nps,34);
}
