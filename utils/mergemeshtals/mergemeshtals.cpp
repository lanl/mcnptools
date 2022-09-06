#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "mcnptools/mergeMeshtals.hpp"
#include "mcnptools/MCNPTools_Config.hpp"

#include "CLParser.hpp" // cyclops command line parser

#ifdef __MCNPTOOLS_BOOST_MPI__
#include <boost/mpi.hpp>
#endif

int main(int argc, char* argv[]) {
  size_t rank = 0;

#ifdef __MCNPTOOLS_BOOST_MPI__
  boost::mpi::environment env;
  boost::mpi::communicator world;
  rank = world.rank();
#endif

  std::string outfile;
  unsigned int verbosity = 1;
  std::vector<std::string> files;

  if( rank == 0 ) {
    cyclops::CLParser<> parser("mergemeshtals");
    parser.MakeHelp();
    parser.Description("$0 statistically merges multiple MCNP MESHTAL files into a single MESHTAL file.");
    parser.Epilog("AUTHOR: Clell J. (CJ) Solomon [csolomon@lanl.gov]");

    cyclops::CLMessageOp version("--version", std::string("Version ") + std::string(MCNPTOOLS_VERSION));
    version.Help("Print version and exit");
    parser.AddOption(version);

    cyclops::CLTrueSwitchOp verbose("--verbose,-v");
    verbose.Help("Increase output verbosity");
    parser.AddOption(verbose);

    cyclops::CLStringValueOp output("--output,-o", "mergemeshtals.out");
    output.Help("Output MESHTAL file name [Default: mergemeshtals.out]");
    parser.AddOption(output);

    cyclops::CLStringPosMultiOp meshtals("MESHTAL");
    meshtals.Required(true);
    meshtals.Help("MESHTAL file names to be merged");
    parser.AddOption(meshtals);

    parser.ParseOptions(argc,argv);

    outfile = output.Value();
    if( verbose.Value() ) verbosity += 1;

    for(size_t i=0; i<meshtals.Count(); i++) {
      std::ifstream test( meshtals.Value(i) );
      if( !test.is_open() ) {
        std::stringstream ss;
        ss << parser.Prog() << " error: file " << meshtals.Value(i) << " does not exist or cannot be opened";
        throw mcnptools::McnpToolsException(ss.str());
      }
      files.push_back( meshtals.Value(i) );
    }
  }

#ifdef __MCNPTOOLS_BOOST_MPI__
  boost::mpi::broadcast(world, outfile, 0);
  mcnptools::parallelMergeMeshtals(outfile, files, verbosity);
#else
  mcnptools::mergeMeshtals(outfile, files, verbosity);
#endif

  return 0;
}
