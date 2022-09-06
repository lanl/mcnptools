#include <iostream>
#include <fstream>
#include <algorithm>

#include "MeshtalFixture.hpp"

TEST_F(MeshtalFixture, MeshtalWrite) {
    // open a file stream and write the meshtal
    std::ofstream out_meshtal("written_meshtal");
    meshtal.WriteHeader( out_meshtal );
    auto meshtal_tallies = meshtal.GetTallyList();
    for( auto tal=meshtal_tallies.begin(); tal!=meshtal_tallies.end(); ++tal) {
        auto tally = meshtal.GetTally(*tal);
        out_meshtal << std::endl;
        tally.WriteToMeshtal( out_meshtal );
    }
    out_meshtal.close();

    // now read in the meshtal we just wrote
    mcnptools::Meshtal written_meshtal("written_meshtal");

    // compare the tally lists
    auto written_meshtal_tallies = written_meshtal.GetTallyList();
    ASSERT_EQ(written_meshtal_tallies.size(), meshtal_tallies.size());
    for( size_t i=0; i<meshtal_tallies.size(); ++i) {
        EXPECT_EQ(written_meshtal_tallies[i], meshtal_tallies[i]);
    }

    // compare individual tallies
    for( auto tal=meshtal_tallies.begin(); tal!=meshtal_tallies.end(); ++tal) {
        auto tally = meshtal.GetTally(*tal);
        auto written_tally = written_meshtal.GetTally(*tal);

        // compare dimension vectors
        auto ibounds = tally.GetXRBounds();
        auto written_ibounds = written_tally.GetXRBounds();
        ASSERT_EQ(ibounds, written_ibounds);

        auto jbounds = tally.GetYZBounds();
        auto written_jbounds = written_tally.GetYZBounds();
        ASSERT_EQ(jbounds, written_jbounds);

        auto kbounds = tally.GetZTBounds();
        auto written_kbounds = written_tally.GetZTBounds();
        ASSERT_EQ(kbounds, written_kbounds);

        auto ebounds = tally.GetEBounds();
        auto written_ebounds = written_tally.GetEBounds();
        ASSERT_EQ(ebounds, written_ebounds);

        auto tbounds = tally.GetTBounds();
        auto written_tbounds = written_tally.GetTBounds();
        ASSERT_EQ(tbounds, written_tbounds);

        auto ibins = tally.GetXRBins();
        auto written_ibins = written_tally.GetXRBins();
        ASSERT_EQ(ibins, written_ibins);

        auto jbins = tally.GetYZBins();
        auto written_jbins = written_tally.GetYZBins();
        ASSERT_EQ(jbins, written_jbins);

        auto kbins = tally.GetZTBins();
        auto written_kbins = written_tally.GetZTBins();
        ASSERT_EQ(kbins, written_kbins);

        auto ebins = tally.GetEBins();
        auto written_ebins = written_tally.GetEBins();
        ASSERT_EQ(ebins, written_ebins);

        auto tbins = tally.GetTBins();
        auto written_tbins = written_tally.GetTBins();
        ASSERT_EQ(tbins, written_tbins);

        // determine range of time/energy bins to check
        size_t tstart = 0;
        size_t tlast = tbounds.size() - 1;
        if( tbounds.size() == 2 ) {
          if( tbounds.at(0) == -1e36 && tbounds.at(1) == 1e36 ) {
            tlast = 1;
          }
        }
        size_t estart = 0;
        size_t elast = ebounds.size() - 1;
        if( ebounds.size() == 2 ) {
          if( ebounds.at(1) == 1e36 ) {
            elast = 1;
          }
        }

        // check *every* value/error to prevent off-by-one issues
        for( size_t t=tstart; t<tlast; ++t) {
            for( size_t e=estart; e<elast; ++e) {
                for( size_t k=0; k<kbounds.size()-1; ++k) {
                    for( size_t j=0; j<jbounds.size()-1; ++j) {
                        for( size_t i=0; i<ibounds.size()-1; ++i) {
                            auto value = tally.GetValue(i, j, k, e, t);
                            auto written_value = written_tally.GetValue(i, j, k, e, t);
                            ASSERT_DOUBLE_EQ(value, written_value);
                            auto error = tally.GetError(i, j, k, e, t);
                            auto written_error = written_tally.GetError(i, j, k, e, t);
                            ASSERT_DOUBLE_EQ(error, written_error);
                        }
                    }
                }
            }
        }

    }

}

