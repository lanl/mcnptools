#include "AscPtracFixture.hpp"

TEST_F(AscPtracFixture, AscPtracReadHistories) {
  std::vector<mcnptools::PtracHistory> hists = ptrac.ReadHistories(73);

  EXPECT_EQ(hists.size(), 73);

  hists = ptrac.ReadHistories(739812);

  EXPECT_EQ(hists.size(), 591-73);

  hists = ptrac.ReadHistories(872163);

  EXPECT_EQ(hists.size(), 0);
}
