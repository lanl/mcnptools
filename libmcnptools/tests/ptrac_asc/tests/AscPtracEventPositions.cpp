#include "AscPtracFixture.hpp"

TEST_F(AscPtracFixture, AscPtracEventType) {
  std::vector<mcnptools::PtracHistory> hists = ptrac.ReadHistories(1);

  mcnptools::PtracEvent e = hists.at(0).GetEvent(0);

  std::vector<double> pos = { e.Get(mcnptools::Ptrac::X),
                              e.Get(mcnptools::Ptrac::Y),
                              e.Get(mcnptools::Ptrac::Z) };

  std::vector<double> check = { 0.0, 0.0, 0.0 };

  ASSERT_EQ(pos.size(),check.size()); 

  for(size_t i=0; i<pos.size(); ++i) 
    EXPECT_EQ(pos[i],check[i]);
}
