#include "MctalTallyFixture.hpp"

TEST_F(MctalTallyFixture, MctalTallyFValues) {
  std::vector<double> value = tally_01.GetValues(mcnptools::MctalTally::MctalTallyBins::F,
                                   mcnptools::MctalTally::TFC,
                                   mcnptools::MctalTally::TFC,
                                   mcnptools::MctalTally::TFC,
                                   mcnptools::MctalTally::TFC,
                                   mcnptools::MctalTally::TFC,
                                   mcnptools::MctalTally::TFC,
                                   mcnptools::MctalTally::TFC);
  //std::vector<double> comp= {0, 0.83252, 0.00202175, 0.834542};
  for(size_t i=0; i<value.size(); ++i)
    EXPECT_EQ(value[i], tally_01.GetValue(i, mcnptools::MctalTally::TFC,
                                          mcnptools::MctalTally::TFC,
                                          mcnptools::MctalTally::TFC,
                                          mcnptools::MctalTally::TFC,
                                          mcnptools::MctalTally::TFC,
                                          mcnptools::MctalTally::TFC,
                                          mcnptools::MctalTally::TFC));
}

TEST_F(MctalTallyFixture, MctalTallyCValues) {
  std::vector<double> value = tally_01.GetValues(mcnptools::MctalTally::MctalTallyBins::C,
                                   mcnptools::MctalTally::TFC,
                                   mcnptools::MctalTally::TFC,
                                   mcnptools::MctalTally::TFC,
                                   mcnptools::MctalTally::TFC,
                                   mcnptools::MctalTally::TFC,
                                   mcnptools::MctalTally::TFC,
                                   mcnptools::MctalTally::TFC);
  //std::vector<double> comp= {0, 0.83252, 0.00202175, 0.834542};
  for(size_t i=0; i<value.size(); ++i)
    EXPECT_EQ(value[i], tally_01.GetValue(mcnptools::MctalTally::TFC,
                                          mcnptools::MctalTally::TFC,
                                          mcnptools::MctalTally::TFC,
                                          mcnptools::MctalTally::TFC,
                                          mcnptools::MctalTally::TFC,i,
                                          mcnptools::MctalTally::TFC,
                                          mcnptools::MctalTally::TFC));
}

TEST_F(MctalTallyFixture, MctalTallyEValues) {
  std::vector<double> value = tally_01.GetValues(mcnptools::MctalTally::MctalTallyBins::E,
                                   mcnptools::MctalTally::TFC,
                                   mcnptools::MctalTally::TFC,
                                   mcnptools::MctalTally::TFC,
                                   mcnptools::MctalTally::TFC,
                                   mcnptools::MctalTally::TFC,
                                   mcnptools::MctalTally::TFC,
                                   mcnptools::MctalTally::TFC);
  std::vector<double> comp= {0., 2.14182e-103 , 0.8324, 0.834542};
  for(size_t i=0; i<value.size(); ++i)
    EXPECT_EQ(value[i],comp[i]);
}

TEST_F(MctalTallyFixture, MctalTallyTValues) {
  std::vector<double> value = tally_01.GetValues(mcnptools::MctalTally::MctalTallyBins::T,
                                   mcnptools::MctalTally::TFC,
                                   mcnptools::MctalTally::TFC,
                                   mcnptools::MctalTally::TFC,
                                   mcnptools::MctalTally::TFC,
                                   mcnptools::MctalTally::TFC,
                                   mcnptools::MctalTally::TFC,
                                   mcnptools::MctalTally::TFC);
  //std::vector<double> comp= {0, 0.83252, 0.00202175, 0.834542};
  for(size_t i=0; i<value.size(); ++i)
    EXPECT_EQ(value[i], tally_01.GetValue(mcnptools::MctalTally::TFC,
                                          mcnptools::MctalTally::TFC,
                                          mcnptools::MctalTally::TFC,
                                          mcnptools::MctalTally::TFC,
                                          mcnptools::MctalTally::TFC,
                                          mcnptools::MctalTally::TFC,
                                          mcnptools::MctalTally::TFC,i));
}

TEST_F(MctalTallyFixture, MctalTallyTErrors) {
  std::vector<double> value = tally_01.GetErrors(mcnptools::MctalTally::MctalTallyBins::T,
                                   mcnptools::MctalTally::TFC,
                                   mcnptools::MctalTally::TFC,
                                   mcnptools::MctalTally::TFC,
                                   mcnptools::MctalTally::TFC,
                                   mcnptools::MctalTally::TFC,
                                   mcnptools::MctalTally::TFC,
                                   mcnptools::MctalTally::TFC);
  //std::vector<double> comp= {0, 0.83252, 0.00202175, 0.834542};
  for(size_t i=0; i<value.size(); ++i)
    EXPECT_EQ(value[i], tally_01.GetError(mcnptools::MctalTally::TFC,
                                          mcnptools::MctalTally::TFC,
                                          mcnptools::MctalTally::TFC,
                                          mcnptools::MctalTally::TFC,
                                          mcnptools::MctalTally::TFC,
                                          mcnptools::MctalTally::TFC,
                                          mcnptools::MctalTally::TFC,i));
}
