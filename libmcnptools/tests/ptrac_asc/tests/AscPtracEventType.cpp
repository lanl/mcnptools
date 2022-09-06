#include "AscPtracFixture.hpp"

TEST_F(AscPtracFixture, AscPtracEventTypes) {
  std::vector<mcnptools::PtracHistory> hists = ptrac.ReadHistories(1);

  std::vector<mcnptools::Ptrac::PtracEventType> types = {
    mcnptools::Ptrac::SRC,
    mcnptools::Ptrac::BNK,
    mcnptools::Ptrac::SUR,
    mcnptools::Ptrac::COL,
    mcnptools::Ptrac::TER 
  };

  for(size_t i=0; i<hists.at(0).GetNumEvents(); i++) {
    mcnptools::PtracEvent e = hists.at(0).GetEvent(i);

    bool event_type_found = false;
    for(size_t j=0; j<types.size(); j++) {
      if( e.Type() == types[j] ) {
        event_type_found = true;
        break;
      }
    }

    EXPECT_TRUE(event_type_found);
  }
}
