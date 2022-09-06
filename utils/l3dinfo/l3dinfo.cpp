#include <vector>
#include <string>
#include <iostream>
#include <fstream>

#include "mcnptools/Lnk3Dnt.hpp"
#include "mcnptools/McnpToolsException.hpp"
#include "mcnptools/MCNPTools_Config.hpp"

#include "CLParser.hpp" // cyclops command line parser

int main(int argc, char* argv[]) {

  cyclops::CLParser<> parser("l3dinfo");
  parser.MakeHelp();
  parser.Description("$0 produces information about LNK3DNT files to stdout");
  parser.Epilog("AUTHOR: Clell J. (CJ) Solomon [csolomon@lanl.gov]");

  cyclops::CLMessageOp version("--version,-v", std::string("Version ") + std::string(MCNPTOOLS_VERSION));
  version.Help("Print version and exit");
  parser.AddOption(version);

  cyclops::CLTrueSwitchOp full("--full,-f");
  full.Help("Produce a full listing of the LNK3DNT contents (can greatly increase runtime)");
  parser.AddOption(full);

  cyclops::CLStringPosMultiOp files("LNK3DNT");
  files.Required(true);
  files.Help("LNK3DNT files about which to produce information");
  parser.AddOption(files);

  parser.ParseOptions(argc,argv);

  // check file existence
  std::vector<std::string> file_names;
  for(size_t i=0; i<files.Count(); i++) {
    if( ! std::ifstream(files.Value(i).c_str()).good() ) {
      throw mcnptools::McnpToolsException("cannot open file " + files.Value(i));
    }
    file_names.push_back(files.Value(i));
  }

  std::string output = mcnptools::l3dinfo(file_names, full.Value());
  std::cout << output;

  return 0;
}
