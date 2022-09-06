#include <iostream>
#include <sstream>
#include <memory>
#include <cmath>
#include <limits>

#include "mcnptools/McnpTools.hpp"
#include "mcnptools/MCNPTools_Config.hpp"

#include "CLParser.hpp" // cyclops command line parser

#include "tiffio.h"

class MctalTallyTiffConverter {
public:
  MctalTallyTiffConverter( mcnptools::MctalTally& tally, const bool direct=false, const bool log=false, const bool transpose=false) {
    // read the tally data
    int dbin = 0;
    if( direct ) dbin=1;

    const std::vector<double> sbins = tally.GetSBins();
    const std::vector<double> cbins = tally.GetCBins();

    m_width = sbins.size() - 1;
    m_height = cbins.size() - 1;

    std::vector<double> float_data;
    float_data.resize( m_width * m_height );
    double max_val = -1e34;
    double min_val =  1e34;
    for(size_t i=0; i<m_width; i++) {
      for(size_t j=0; j<m_height; j++) {
        size_t idx = i*m_height + j;
        if( transpose )
          idx = i + m_width*j;
          
        float_data[idx] = tally.GetValue( mcnptools::MctalTally::TFC,
                                          dbin,
                                          mcnptools::MctalTally::TFC,
                                          i,
                                          mcnptools::MctalTally::TFC,
                                          j,
                                          mcnptools::MctalTally::TFC,
                                          mcnptools::MctalTally::TFC);

        // make logarithmic
        if( log ) {
          if( float_data[idx] > 0.0 ) {
            float_data[idx] = std::log10( float_data[idx] );
          }
          else {
            float_data[idx] = 1e-34;
          }
        }

        if( float_data[idx] > max_val ) max_val = float_data[idx];
        if( float_data[idx] < min_val ) min_val = float_data[idx];
      }
    }
    
    // normalize the float data
    for(size_t i=0; i<float_data.size(); i++) {
      float_data[i] = (float_data[i] - min_val) / (max_val - min_val);
    }

    // set the 32-bit image data
    m_image_data.resize( float_data.size() );
    for(size_t i=0; i<float_data.size(); i++) {
      m_image_data[i] = float_data[i] * std::numeric_limits<uint8_t>::max();
    }

    if( transpose ) {
      size_t tmp = m_width;
      m_width = m_height;
      m_height = tmp;
    }
  };

  void WriteTiff(std::string filename) {
    TIFF *output_image;
    if( (output_image = TIFFOpen(filename.c_str(), "w")) == NULL ) {
      std::cerr << "Unable to write tif file: " << filename << std::endl;
    }

    TIFFSetField(output_image, TIFFTAG_IMAGEWIDTH, m_height);
    TIFFSetField(output_image, TIFFTAG_IMAGELENGTH, m_width);
    TIFFSetField(output_image, TIFFTAG_BITSPERSAMPLE, 8);
    TIFFSetField(output_image, TIFFTAG_SAMPLESPERPIXEL, 1);
    TIFFSetField(output_image, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
    
    TIFFSetField(output_image, TIFFTAG_COMPRESSION, COMPRESSION_DEFLATE);
    TIFFSetField(output_image, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
                                   
    // Write the information to the file
    TIFFWriteEncodedStrip(output_image, 0, &m_image_data[0], m_width*m_height);
    
    // Close the file
    TIFFClose(output_image);
  }

private:
  std::vector<uint8_t> m_image_data;
  size_t m_width;
  size_t m_height;
};

int main(int argc, char* argv[]) {

  cyclops::CLParser<> parser("mctal2rad");
  parser.MakeHelp();
  parser.Description("$0 converts an image tally from an MCNP MCTAL file into a TIFF image");
  parser.Epilog("AUTHOR: Clell J. (CJ) Solomon [csolomon@lanl.gov]");

  cyclops::CLMessageOp version("--version,-v", std::string("Version ") + std::string(MCNPTOOLS_VERSION));
  version.Help("Print version and exit");
  parser.AddOption(version);

  cyclops::CLTrueSwitchOp log("--log,-l");
  log.Help("Produce an image of the log of the MCTAL values");
  parser.AddOption(log);

  cyclops::CLTrueSwitchOp direct("--direct,-d");
  direct.Help("Produce an image of the direct contribution");
  parser.AddOption(direct);

  cyclops::CLTrueSwitchOp transpose("--transpose,-t");
  transpose.Help("Transpose the image");
  parser.AddOption(transpose);

  cyclops::CLStringPosValueOp mctalfile("MCTAL");
  mctalfile.Required(true);
  mctalfile.Help("MCTAL file containing one or more image tallies");
  parser.AddOption(mctalfile);

  cyclops::CLIntPosMultiOp tallies("TALLY",0);
  tallies.Help("Tally number for which to produce the images");
  parser.AddOption(tallies);

  parser.ParseOptions(argc,argv);

  mcnptools::Mctal mctal(mctalfile.Value());
  std::vector<int> tlist;
  if( tallies.Count() == 0 ) {
    tlist = mctal.GetTallyList();
  }
  else {
    for(size_t i=0; i<tallies.Count(); i++) {
      int tnum = tallies.Value(i);
      tlist.push_back(tnum);
    }
  }

  for(auto it=tlist.begin(); it!=tlist.end(); it++) {
    mcnptools::MctalTally tally = mctal.GetTally(*it);
    if( tally.GetNextEventEstimatorType() < 3 ) {
      std::cerr << parser.Prog() << " warning: tally " << *it << " of MCTAL file " << mctalfile.Value()
                << " is not an image tally...skipping" << std::endl;
      continue;
    }

    if( direct.Value() ) {
      if( tally.GetDBins().size() < 2 ) {
        std::cerr << parser.Prog() << " warning: tally " << *it << " of MCTAL file " << mctalfile.Value()
                  << " does not have a direct-only tally...skipping" << std::endl;
        
        continue;
      }
    }
    
    std::stringstream oss;
    oss << mctalfile.Value() << "_" << *it << ".tif";

    MctalTallyTiffConverter tiffer(tally, direct.Value(), log.Value(), transpose.Value());
    tiffer.WriteTiff(oss.str());
  }

  return 0;
}
