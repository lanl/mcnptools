#include "mcnptools/mergeMeshtals.hpp"

#include <iostream>

#ifdef __MCNPTOOLS_BOOST_MPI__
#include <boost/mpi.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/map.hpp>
#include <functional>
#endif

namespace mcnptools {

Meshtal mergeMeshtals(const std::string& name, const std::vector<std::string> meshtal_names, const unsigned int verbosity) {
  std::ofstream outmeshtal(name.c_str());

  std::map<int,std::streamoff> new_tallies;

  if( verbosity > 0 )
    std::cerr << "Indexing MESHTAL files" << std::endl;
  if( verbosity > 1 )
    std::cerr << "Indexing MESHTAL file \'" << meshtal_names.at(0) << "\'" << std::endl;
  Meshtal base(meshtal_names.at(0));
  std::vector<Meshtal> meshtals;
  for(unsigned int m=1; m<meshtal_names.size(); m++) {
    if( verbosity > 1 )
      std::cerr << "Indexing MESHTAL file \'" << meshtal_names.at(m) << "\'" << std::endl;
    meshtals.push_back(Meshtal(meshtal_names.at(m)));
  }

  const std::vector<int> tallies = base.GetTallyList();

  long int tmp_nps = base.GetNps();
  for(unsigned int m=0; m<meshtals.size(); m++) {
    base.m_nps += meshtals.at(m).GetNps();
  }
  base.WriteHeader(outmeshtal);
  base.m_nps = tmp_nps;

  // iterate over all tallies
  if( verbosity > 0 )
    std::cerr << "Merging tallies" << std::endl;
  for(std::vector<int>::const_iterator tal=tallies.begin(); tal!=tallies.end(); tal++) {
    if( verbosity > 1 )
      std::cerr << "Merging tally " << *tal << std::endl;

    MeshtalTally base_tal = base.GetTally(*tal);

    for(unsigned int m=0; m<meshtals.size(); m++) {
      MeshtalTally mesh_tal = meshtals.at(m).GetTally(*tal);

      try {
        base_tal &= mesh_tal;
      }
      catch(McnpToolsException) {
        std::stringstream ss;
        ss << "tally " << *tal << " is nonconforming in MESHTAL files " << meshtals.at(0) << " and " << meshtals.at(m);
        throw McnpToolsException(ss.str());
      }
    }

    new_tallies[*tal] = outmeshtal.tellp();
    base_tal.WriteToMeshtal(outmeshtal);
  }

  outmeshtal.close();

  return Meshtal(name, new_tallies);
}

#ifdef __MCNPTOOLS_BOOST_MPI__
Meshtal parallelMergeMeshtals(const std::string& name, const std::vector<std::string> in_meshtal_names, const unsigned int in_verbosity) {
  boost::mpi::communicator world;
  size_t rank = world.rank();

  std::vector<std::string> meshtal_names;
  unsigned int verbosity;
  size_t size;

  std::ofstream outmeshtal;
  if( rank == 0 ) {
    outmeshtal.open(name.c_str());
    meshtal_names = in_meshtal_names;
    verbosity = in_verbosity;
    size = std::min( static_cast<size_t>(world.size()), 
                     static_cast<size_t>(in_meshtal_names.size()) );
  }

  boost::mpi::broadcast(world, meshtal_names, 0);
  boost::mpi::broadcast(world, verbosity, 0);
  boost::mpi::broadcast(world, size, 0);

  // create a local communicator the minimum of size
  boost::mpi::communicator local = world.split( rank < size ? 1 : 2 );
  size_t lrank = local.rank();

  std::map<int,std::streamoff> new_tallies;

  if( rank < size ) {

    if( lrank == 0 && verbosity > 0 )
      std::cerr << "Indexing MESHTAL files" << std::endl;

    if( verbosity > 1 )
      std::cerr << "Task " << lrank << " indexing MESHTAL file \'" << meshtal_names.at(0) << "\'" << std::endl;

    Meshtal base(meshtal_names.at(lrank));

    std::vector<Meshtal> meshtals;
    for(unsigned int m=lrank+size; m<meshtal_names.size(); m+=size) {
      if( verbosity > 1 )
        std::cerr << "Task " << lrank << " indexing MESHTAL file \'" << meshtal_names.at(m) << "\'" << std::endl;
      meshtals.push_back(Meshtal(meshtal_names.at(m)));
    }

    // build the total number of histories
    int64_t tmp_nps = base.GetNps();
    for(unsigned int m=0; m<meshtals.size(); m++) {
      base.m_nps += meshtals.at(m).GetNps();
    }

    if( rank == 0 ) {
      int64_t tmp;
      boost::mpi::reduce<int64_t>(local, base.m_nps, tmp, std::plus<int64_t>(), 0);
      base.m_nps = tmp;
      base.WriteHeader(outmeshtal);
    }
    else {
      boost::mpi::reduce<int64_t>(local, base.m_nps, std::plus<int64_t>(), 0);
    }
    base.m_nps = tmp_nps;

    // iterate over all tallies
    std::vector<int> tallies;
    if( lrank == 0 )
      tallies = base.GetTallyList();

    boost::mpi::broadcast(local, tallies, 0);

    if( lrank == 0 && verbosity > 0 )
      std::cerr << "Merging tallies" << std::endl;

    for(std::vector<int>::const_iterator tal=tallies.begin(); tal!=tallies.end(); tal++) {
      if( lrank == 0 && verbosity > 1 )
        std::cerr << "Merging tally " << *tal << std::endl;

      MeshtalTally base_tal = base.GetTally(*tal);

      for(unsigned int m=0; m<meshtals.size(); m++) {
        MeshtalTally mesh_tal = meshtals.at(m).GetTally(*tal);

        try {
          base_tal &= mesh_tal;
        }
        catch(McnpToolsException) {
          std::stringstream ss;
          ss << "tally " << *tal << " is nonconforming in MESHTAL files " << meshtals.at(0) << " and " << meshtals.at(m);
          throw McnpToolsException(ss.str());
        }
      }

      if( lrank == 0 ) {
        MeshtalTally tmp;
        boost::mpi::reduce(local, base_tal, tmp, mergeMeshtalTallies, 0);
        base_tal = tmp;
      }
      else {
        boost::mpi::reduce(local, base_tal, mergeMeshtalTallies, 0);
      }

      if( lrank == 0 ) {
        new_tallies[*tal] = outmeshtal.tellp();
        base_tal.WriteToMeshtal(outmeshtal);
      }
    }
  }

  world.barrier();
  boost::mpi::broadcast(world, new_tallies, 0);

  if( rank == 0 )
    outmeshtal.close();

  return Meshtal(name, new_tallies);
}
#endif

}
