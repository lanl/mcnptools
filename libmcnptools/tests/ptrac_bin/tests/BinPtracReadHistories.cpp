#include "BinPtracFixture.hpp"

TEST_F(BinPtracFixture, BinPtracReadHistories) {
  std::vector<mcnptools::PtracHistory> hists = ptrac.ReadHistories(73);

  EXPECT_EQ(hists.size(), 73);

  hists = ptrac.ReadHistories(739812);

  EXPECT_EQ(hists.size(), 591-73);

  hists = ptrac.ReadHistories(872163);

  EXPECT_EQ(hists.size(), 0);
}
