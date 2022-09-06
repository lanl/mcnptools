#include "mcnptools/McnpTools.hpp"
#include <iostream>
#include <vector>

int main() {

    // construct the mctal class from mctal file "example_mctal_1.mcnp.mctal"
    mcnptools::Mctal m("example_mctal_1.mcnp.mctal");

    int tfc = mcnptools::MctalTally::TFC; // alias for -1

    // get tally 4 from the mctal file
    mcnptools::MctalTally t4 = m.GetTally(4);

    auto E = mcnptools::MctalTally::MctalTallyBins::E;
    //  using the TFC bin for all other bins
    std::vector<double> t4_evals = t4.GetValues(E,tfc,tfc,tfc,tfc,tfc,tfc,tfc);
    for (double it : t4_evals) {
      std::cout << it << std::endl;
    }

    return 0;
}
