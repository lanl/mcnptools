#include "mcnptools/mergeMctals.hpp"

#include <iostream>

#ifdef __MCNPTOOLS_BOOST_MPI__
#include <boost/mpi.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/map.hpp>
#include <functional>
#endif

namespace mcnptools {

Mctal mergeMctals(const std::string& name, const std::vector<std::string> mctal_names, const unsigned int verbosity) {
  std::ofstream outmctal(name.c_str());

  std::streamoff new_kpos = 0;
  std::map<int,std::streamoff> new_talloc;

  if( verbosity > 0 )
    std::cerr << "Indexing MCTAL files" << std::endl;
  if( verbosity > 1 )
    std::cerr << "Indexing MCTAL file \'" << mctal_names.at(0) << "\'" << std::endl;
  Mctal base(mctal_names.at(0));

  std::vector<Mctal> mctals;
  for(unsigned int m=1; m<mctal_names.size(); m++) {
    if( verbosity > 1 )
      std::cerr << "Indexing MCTAL file \'" << mctal_names.at(m) << "\'" << std::endl;
    mctals.push_back(Mctal(mctal_names.at(m)));
  }

  const std::vector<int> tallies = base.GetTallyList();

  long int tmp_nps = base.GetNps();
  for(unsigned int m=0; m<mctals.size(); m++) {
    base.m_nps += mctals.at(m).GetNps();
    base.m_randoms += mctals.at(m).GetRandoms();
  }
  base.WriteHeader(outmctal);
  base.m_nps = tmp_nps;

  if( verbosity > 0 && tallies.size() > 0 )
    std::cerr << "Merging tallies" << std::endl;
  // iterate over all tallies
  for(std::vector<int>::const_iterator tal=tallies.begin(); tal!=tallies.end(); tal++) {
    if( verbosity > 1 )
      std::cerr << "Merging tally " << *tal << std::endl;

    MctalTally base_tal = base.GetTally(*tal);

    for(unsigned int m=0; m<mctals.size(); m++) {
      MctalTally mct_tal = mctals.at(m).GetTally(*tal);

      try {
        base_tal &= mct_tal;
      }
      catch(McnpToolsException) {
        std::stringstream ss;
        ss << "tally " << *tal << " is nonconforming in MCTAL files " << mctals.at(0) << " and " << mctals.at(m);
        throw McnpToolsException(ss.str());
      }
    }

    new_talloc[*tal] = outmctal.tellp();
    base_tal.WriteToMctal(outmctal);
  }

  // combine kcode data if present
  if( base.m_kpos != std::streamoff(0) ) {
    if( verbosity > 0 )
      std::cerr << "Merging kcode data" << std::endl;
    
    MctalKcode base_kc = base.GetKcode();

    for(unsigned int m=0; m<mctals.size(); m++) {
      MctalKcode mct_kc = mctals.at(m).GetKcode();

      try {
        base_kc &= mct_kc;
      }
      catch(McnpToolsException) {
        std::stringstream ss;
        ss << "kcode is nonconforming in MCTAL files " << mctals.at(0) << " and " << mctals.at(m);
        throw McnpToolsException(ss.str());
      }
    }

    new_kpos = outmctal.tellp();
    base_kc.WriteToMctal(outmctal);
  }

  outmctal.close();
  
  return Mctal(name, tallies, new_talloc, new_kpos);
}

#ifdef __MCNPTOOLS_BOOST_MPI__
Mctal parallelMergeMctals(const std::string& name, const std::vector<std::string> in_mctal_names, const unsigned int in_verbosity) {
  boost::mpi::communicator world;
  size_t rank = world.rank();

  std::vector<std::string> mctal_names;
  unsigned int verbosity;
  size_t size;

  std::ofstream outmctal;
  if( rank == 0 ) { 
    outmctal.open(name.c_str());
    mctal_names=in_mctal_names;
    verbosity = in_verbosity;
    size = std::min( static_cast<size_t>(world.size()), 
                     static_cast<size_t>(in_mctal_names.size()) );
  }
    
  boost::mpi::broadcast(world, mctal_names, 0);
  boost::mpi::broadcast(world, verbosity, 0);
  boost::mpi::broadcast(world, size, 0);

  // create a local communicator the minimum of size
  boost::mpi::communicator local = world.split( rank < size ? 1 : 2 );
  size_t lrank = local.rank();

  std::streamoff new_kpos = 0;
  std::vector<int> tallies;
  std::map<int,std::streamoff> new_talloc;

  if( rank < size ) {

    if( lrank == 0 && verbosity > 0 )
      std::cerr << "Indexing MCTAL files" << std::endl;

    if( verbosity > 1 )
      std::cerr << "Task " << lrank << " indexing MCTAL file \'" << mctal_names.at(rank) << "\'" << std::endl;
    
    Mctal base(mctal_names.at(lrank));

    std::vector<Mctal> mctals;
    for(size_t m=lrank+size; m<mctal_names.size(); m+=size) {
      if( verbosity > 1 )
        std::cerr << "Task " << lrank << " indexing MCTAL file \'" << mctal_names.at(m) << "\'" << std::endl;
      mctals.push_back(Mctal(mctal_names.at(m)));
    }

    // build the total number of histories and random numbers used
    long unsigned int tmp_nps = base.GetNps();
    for(unsigned int m=0; m<mctals.size(); m++) {
      base.m_nps += mctals.at(m).GetNps();
      base.m_randoms += mctals.at(m).GetRandoms();
    }

    if( lrank == 0 ) {
      long unsigned int tmp;
      boost::mpi::reduce(local, base.m_nps, tmp, std::plus<long int>(), 0);
      base.m_nps = tmp;
      boost::mpi::reduce(local, base.m_randoms, tmp, std::plus<long int>(), 0);
      base.m_randoms = tmp;
      base.WriteHeader(outmctal);
    }
    else {
      boost::mpi::reduce(local, base.m_nps, std::plus<long int>(), 0);
      boost::mpi::reduce(local, base.m_randoms, std::plus<long int>(), 0);
    }
    base.m_nps = tmp_nps;

    // iterate over all tallies
    if( lrank == 0 ) 
      tallies = base.GetTallyList();

    boost::mpi::broadcast(local, tallies, 0);

    if( lrank == 0 && verbosity > 0 && tallies.size() > 0 )
      std::cerr << "Merging tallies" << std::endl;

    for(std::vector<int>::iterator tal=tallies.begin(); tal!=tallies.end(); tal++) {
      if( lrank == 0 && verbosity > 1 )
        std::cerr << "Merging tally " << *tal << std::endl;

      MctalTally base_tal = base.GetTally(*tal);

      for(unsigned int m=0; m<mctals.size(); m++) {
        MctalTally mct_tal = mctals.at(m).GetTally(*tal);

        try {
          base_tal &= mct_tal;
        }
        catch(McnpToolsException) {
          std::stringstream ss;
          ss << "tally " << *tal << " is nonconforming in MCTAL files " << mctals.at(0) << " and " << mctals.at(m);
          throw McnpToolsException(ss.str());
        }
      }

      if( lrank == 0 ) {
        MctalTally tmp;
        boost::mpi::reduce(local, base_tal, tmp, mergeMctalTallies, 0);
        base_tal = tmp;
      }
      else {
        boost::mpi::reduce(local, base_tal, mergeMctalTallies, 0);
      }

      if( lrank == 0 ) {
        new_talloc[*tal] = outmctal.tellp();
        base_tal.WriteToMctal(outmctal);
      }
    }

    // combine kcode data if present
    if( base.m_kpos != std::streamoff(0) ) {
      if( lrank == 0 && verbosity > 0 )
        std::cerr << "Merging kcode data" << std::endl;
      
      MctalKcode base_kc = base.GetKcode();

      for(unsigned int m=0; m<mctals.size(); m++) {
        MctalKcode mct_kc = mctals.at(m).GetKcode();

        try {
          base_kc &= mct_kc;
        }
        catch(McnpToolsException) {
          std::stringstream ss;
          ss << "kcode is nonconforming in MCTAL files " << mctals.at(0) << " and " << mctals.at(m);
          throw McnpToolsException(ss.str());
        }
      }

      if( lrank == 0 ) {
        MctalKcode tmp;
        boost::mpi::reduce(local, base_kc, tmp, mergeMctalKcodes, 0);
        base_kc = tmp;
      }
      else {
        boost::mpi::reduce(local, base_kc, mergeMctalKcodes, 0);
      }

      if( lrank == 0 ) {
        new_kpos = outmctal.tellp();
        base_kc.WriteToMctal(outmctal);
      }
    }
  } // end if( lrank < size )
  world.barrier();
  boost::mpi::broadcast(world, tallies, 0);
  boost::mpi::broadcast(world, new_talloc, 0);
  boost::mpi::broadcast(world, new_kpos, 0);

  if( rank == 0 ) outmctal.close();
  
  return Mctal(name, tallies, new_talloc, new_kpos);
}
#endif

} // namespace
