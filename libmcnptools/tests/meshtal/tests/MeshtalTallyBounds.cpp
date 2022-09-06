#include "MeshtalTallyFixture.hpp"

void check_xyz_tallies(const std::vector<mcnptools::MeshtalTally*>& xyz_tallies) {
  const std::vector<double> cbounds = { -10.00, -6.00, -2.00, 2.00, 6.00, 10.00};
  const std::vector<double> cbins   = {  -8.00, -4.00,  0.00, 4.00, 8.00};

  for(auto it=xyz_tallies.begin(); it!=xyz_tallies.end(); ++it) {
    mcnptools::MeshtalTally& tal = *(*it);

    // x direction
    std::vector<double> bounds = tal.GetXRBounds();
    std::vector<double> bins = tal.GetXRBins();

    ASSERT_EQ(bounds.size(), cbounds.size());
    ASSERT_EQ(bins.size(), cbins.size());
  
    for(size_t i=0; i<bounds.size(); ++i) {
      EXPECT_NEAR(bounds[i],cbounds[i],1e-6);
      if( i<bins.size() )
        EXPECT_NEAR(bins[i],cbins[i],1e-6);
    }
        
    // y direction
    bounds = tal.GetYZBounds();
    bins = tal.GetYZBins();

    ASSERT_EQ(bounds.size(), cbounds.size());
    ASSERT_EQ(bins.size(), cbins.size());
  
    for(size_t i=0; i<bounds.size(); ++i) {
      EXPECT_NEAR(bounds[i],cbounds[i],1e-6);
      if( i<bins.size() )
        EXPECT_NEAR(bins[i],cbins[i],1e-6);
    }

    // z direction
    bounds = tal.GetZTBounds();
    bins = tal.GetZTBins();

    ASSERT_EQ(bounds.size(), cbounds.size());
    ASSERT_EQ(bins.size(), cbins.size());
  
    for(size_t i=0; i<bounds.size(); ++i) {
      EXPECT_NEAR(bounds[i],cbounds[i],1e-6);
      if( i<bins.size() )
        EXPECT_NEAR(bins[i],cbins[i],1e-6);
    }
  }
}

void check_rzt_tallies(const std::vector<mcnptools::MeshtalTally*>& rzt_tallies) {
  const std::vector<double> rbounds = {  0.00,  2.50,  5.00,  7.50, 10.00 };
  const std::vector<double> zbounds = {  0.00,  2.00,  4.00,  6.00,  8.00, 10.00 };
  const std::vector<double> tbounds = { 0.000, 0.250, 0.500, 0.750, 1.000 };
  const std::vector<double> rbins   = {  1.25,  3.75,  6.25,  8.75 };
  const std::vector<double> zbins   = {  1.00,  3.00,  5.00,  7.00, 9.00};
  const std::vector<double> tbins   = { 0.125, 0.375, 0.625, 0.875 };

  for(auto it=rzt_tallies.begin(); it!=rzt_tallies.end(); ++it) {
    mcnptools::MeshtalTally& tal = *(*it);

    // x direction
    std::vector<double> bounds = tal.GetXRBounds();
    std::vector<double> bins = tal.GetXRBins();

    ASSERT_EQ(bounds.size(), rbounds.size());
    ASSERT_EQ(bins.size(), rbins.size());
  
    for(size_t i=0; i<bounds.size(); ++i) {
      EXPECT_NEAR(bounds[i],rbounds[i],1e-6);
      if( i<bins.size() )
        EXPECT_NEAR(bins[i],rbins[i],1e-6);
    }
        
    // y direction
    bounds = tal.GetYZBounds();
    bins = tal.GetYZBins();

    ASSERT_EQ(bounds.size(), zbounds.size());
    ASSERT_EQ(bins.size(), zbins.size());
  
    for(size_t i=0; i<bounds.size(); ++i) {
      EXPECT_NEAR(bounds[i],zbounds[i],1e-6);
      if( i<bins.size() )
        EXPECT_NEAR(bins[i],zbins[i],1e-6);
    }

    // z direction
    bounds = tal.GetZTBounds();
    bins = tal.GetZTBins();

    ASSERT_EQ(bounds.size(), tbounds.size());
    ASSERT_EQ(bins.size(), tbins.size());
  
    for(size_t i=0; i<bounds.size(); ++i) {
      EXPECT_NEAR(bounds[i],tbounds[i],1e-6);
      if( i<bins.size() )
        EXPECT_NEAR(bins[i],tbins[i],1e-6);
    }
  }
}

void check_e_tallies(const std::vector<mcnptools::MeshtalTally*>& e_tallies) {
  const std::vector<double> ebounds = {0.00E+00, 1.00E-09, 1.00E-03, 1.00E+03};
  const std::vector<double> ebins = {1.00E-09, 1.00E-03, 1.00E+03};

  for(auto it=e_tallies.begin(); it!=e_tallies.end(); ++it) {
    mcnptools::MeshtalTally& tal = *(*it);

    std::vector<double> bounds = tal.GetEBounds();
    std::vector<double> bins = tal.GetEBins();

    ASSERT_EQ(bounds.size(), ebounds.size());
    ASSERT_EQ(bins.size(), ebins.size());

    for(size_t i=0; i<bounds.size(); ++i) {
      EXPECT_NEAR(bounds[i], ebounds[i], 1e-6);
      if( i<bins.size() )
        EXPECT_NEAR(bins[i], ebins[i], 1e-6);
    }
  }
}

void check_t_tallies(const std::vector<mcnptools::MeshtalTally*>& t_tallies) {
  const std::vector<double> tbounds = {-1.00E+36, 0.00E+00, 1.00E+01, 2.00E+01};
  const std::vector<double> tbins = {0.00E+00, 1.00E+01, 2.00E+01};

  for(auto it=t_tallies.begin(); it!=t_tallies.end(); ++it) {
    mcnptools::MeshtalTally& tal = *(*it);

    std::vector<double> bounds = tal.GetTBounds();
    std::vector<double> bins = tal.GetTBins();

    ASSERT_EQ(bounds.size(), tbounds.size());
    ASSERT_EQ(bins.size(), tbins.size());

    for(size_t i=0; i<bounds.size(); ++i) {
      EXPECT_NEAR(bounds[i], tbounds[i], 1e-6);
      if( i<bins.size() )
        EXPECT_NEAR(bins[i], tbins[i], 1e-6);
    }
  }
}


TEST_F(MeshtalTallyFixture, MeshtalTallyBounds) {
  std::vector<mcnptools::MeshtalTally*> xyz_tallies = { &tally_14, &tally_24, &tally_34, &tally_44 };
  check_xyz_tallies(xyz_tallies);

  std::vector<mcnptools::MeshtalTally*> rzt_tallies = { &tally_54, &tally_64, &tally_74, &tally_84 };
  check_rzt_tallies(rzt_tallies);

  std::vector<mcnptools::MeshtalTally*> e_tallies = { &tally_24, &tally_44, &tally_64, &tally_84 };
  check_e_tallies(e_tallies);

  std::vector<mcnptools::MeshtalTally*> t_tallies = { &tally_34, &tally_44, &tally_74, &tally_84 };
  check_t_tallies(t_tallies);
}
