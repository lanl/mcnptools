#include <iostream>
#include <string>
#include <sstream>

#include "mcnptools/Lnk3Dnt.hpp"
#include "mcnptools/MCNPTools_Config.hpp"

#include "CLParser.hpp" // cyclops command line parser

int main(int argc, char* argv[]) {

  cyclops::CLParser<> parser("l3dcoarsen");
  parser.MakeHelp();
  parser.Description("$0 coarsens a LNK3DNT file mesh by specified factors");
  parser.Epilog("AUTHOR: Clell J. (CJ) Solomon [csolomon@lanl.gov]");

  cyclops::CLMessageOp version("--version,-v", std::string("Version ") + std::string(MCNPTOOLS_VERSION));
  version.Help("Print version and exit");
  parser.AddOption(version);

  cyclops::CLFalseSwitchOp novoid("--novoid,-n");
  novoid.Help("Make voids material \'0\' rather than the assumed material \'1\' (not recommended)");
  parser.AddOption(novoid);

  cyclops::CLIntValueOp ifact("--ifact,-i", 1);
  ifact.Help("Factor by which to coarsen in the first mesh dimension");
  parser.AddOption(ifact);

  cyclops::CLIntValueOp jfact("--jfact,-j", 1);
  jfact.Help("Factor by which to coarsen in the second mesh dimension (if applicable)");
  parser.AddOption(jfact);

  cyclops::CLIntValueOp kfact("--kfact,-k", 1);
  kfact.Help("Factor by which to coarsen in the third mesh dimension (if applicable)");
  parser.AddOption(kfact);

  cyclops::CLIntValueOp maxmats("--maxmats,-m", 0);
  maxmats.Help("Maximum number of materials to keep include on the coarsened LNK3DNT file (default: same as original)");
  parser.AddOption(maxmats);

  cyclops::CLStringPosValueOp l3dname("LNK3DNT");
  l3dname.Required(true);
  l3dname.Help("LNK3DNT file name to coarsen");
  parser.AddOption(l3dname);

  cyclops::CLStringPosValueOp l3dout("OUTPUT", "lnk3dnt.coarse");
  l3dout.Help("coarsened LNK3DNT output name (Default: lnk3dnt.coarse)");
  parser.AddOption(l3dout);

  parser.ParseOptions(argc,argv);

  mcnptools::Lnk3Dnt l3d( l3dname.Value() );
  l3d.ReadMaterials();

  l3d.Coarsen(ifact.Value(), jfact.Value(), kfact.Value() , maxmats.Value(), novoid.Value());

  l3d.Save( l3dout.Value() );

  return 0;
}
