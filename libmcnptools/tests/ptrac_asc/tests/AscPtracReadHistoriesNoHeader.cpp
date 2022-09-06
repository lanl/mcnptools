#include "AscPtracFixture.hpp"

TEST_F(AscPtracFixtureNoHeader, AscPtracReadHistoriesNoHeader) {
  std::vector<mcnptools::PtracHistory> hists = ptrac_prdmp_neg_1.ReadHistories(73);

  mcnptools::PtracEvent e = hists.at(0).GetEvent(0);

  EXPECT_EQ(e.Get(mcnptools::Ptrac::ENERGY), 2.0);
  EXPECT_EQ(hists.size(), 73);

  hists = ptrac_prdmp_neg_1.ReadHistories(739812);

  EXPECT_EQ(hists.size(), 591-73);
  
  hists = ptrac_prdmp_neg_1.ReadHistories(872163);

  EXPECT_EQ(hists.size(), 0);
}
