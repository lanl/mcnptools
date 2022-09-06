#include <iostream>
#include <sstream>
#include <vector>

#include "mcnptools/mergeMctals.hpp"
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
  std::vector<std::string> files;
  unsigned int verbosity = 1;

  // process command line options if master
  if( rank == 0 ) {
    cyclops::CLParser<> parser("mergemctals");
    parser.MakeHelp();
    parser.Description("$0 statistically merges multiple MCNP MCTAL files into a single MCTAL file.");
    parser.Epilog("AUTHOR: Clell J. (CJ) Solomon [csolomon@lanl.gov]");

    cyclops::CLMessageOp version("--version", std::string("Version ") + std::string(MCNPTOOLS_VERSION));
    version.Help("Print version and exit");
    parser.AddOption(version);

    cyclops::CLTrueSwitchOp verbose("--verbose,-v");
    verbose.Help("Increase output verbosity");
    parser.AddOption(verbose);

    cyclops::CLStringValueOp output("--output,-o", "mergemctals.out");
    output.Help("Output MCTAL file name [Default: mergemctals.out]");
    parser.AddOption(output);

    cyclops::CLStringPosMultiOp mctals("MCTAL");
    mctals.Required(true);
    mctals.Help("MCTAL file names to be merged");
    parser.AddOption(mctals);

    parser.ParseOptions(argc,argv);
  
    outfile = output.Value();
    if( verbose.Value() ) verbosity += 1;

    for( size_t i=0; i<mctals.Count(); i++ ) {
      std::ifstream test( mctals.Value(i) );
      if( !test.is_open() ) {
        std::stringstream ss;
        ss << parser.Prog() << " error: file " << mctals.Value(i) << " does not exist or cannot be opened";
        throw mcnptools::McnpToolsException(ss.str());
      }
      files.push_back( mctals.Value(i) );
    }
  }

#ifdef __MCNPTOOLS_BOOST_MPI__
  boost::mpi::broadcast(world, outfile, 0);
  mcnptools::parallelMergeMctals(outfile, files, verbosity);
#else
  mcnptools::mergeMctals(outfile, files, verbosity);
#endif

  return 0;
}
