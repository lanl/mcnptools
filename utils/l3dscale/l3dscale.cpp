#include <iostream>
#include <string>
#include <sstream>

#include "mcnptools/Lnk3Dnt.hpp"
#include "mcnptools/McnpToolsException.hpp"
#include "mcnptools/MCNPTools_Config.hpp"

#include "CLParser.hpp" // cyclops command line parser

int main(int argc, char* argv[]) {

  cyclops::CLParser<> parser("l3dscale");
  parser.MakeHelp();
  parser.Description("$0 scales the dimensions of a  LNK3DNT file");
  parser.Epilog("AUTHOR: Clell J. (CJ) Solomon [csolomon@lanl.gov]");

  cyclops::CLMessageOp version("--version,-v", std::string("Version ") + std::string(MCNPTOOLS_VERSION));
  version.Help("Print version and exit");
  parser.AddOption(version);

  cyclops::CLStringPosValueOp file("LNK3DNT");
  file.Required(true);
  file.Help("LNK3DNT file to be scaled");
  parser.AddOption(file);

  cyclops::CLDoublePosValueOp scale("FACTOR");
  scale.Required(true);
  scale.Help("Scaling factor to be applied to the file");
  parser.AddOption(scale);

  cyclops::CLStringPosValueOp output("OUTPUT");
  output.Help("Output LNK3DNT file name [Default: LNK3DNT.scaled]");
  parser.AddOption(output);

  parser.ParseOptions(argc,argv);

  std::string oname = output.Value();
  if( oname.size() == 0 )
    oname = file.Value() + ".scaled";

  // check file existence
  if( ! std::ifstream(file.Value().c_str()).good() ) {
    throw mcnptools::McnpToolsException("cannot open file " + file.Value());
  }

  mcnptools::Lnk3Dnt l3d(file.Value());
  l3d.ReadMaterials();

  l3d.Scale( scale.Value() );

  l3d.Save(oname);

  return 0;
}
