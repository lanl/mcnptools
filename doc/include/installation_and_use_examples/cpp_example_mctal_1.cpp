#include "mcnptools/McnpTools.hpp"
#include <iostream>
#include <vector>

int main() {

    // construct the mctal class from mctal file "example_mctal_1.mcnp.mctal"
    mcnptools::Mctal m("example_mctal_1.mcnp.mctal");

    int tfc = mcnptools::MctalTally::TFC; // alias for -1

    // get tally 4 from the mctal file
    mcnptools::MctalTally t4 = m.GetTally(4);

    // get the energy bins of tally 4
    std::vector<double> t4_e = t4.GetEBins();

    // loop over energy bin indices to store and print tally bin value
    // using the TFC bin for all other bins
    std::vector<double> t4_evals(t4_e.size()); // storage for tally values
    for (unsigned int i = 0; i < t4_e.size(); i++) {
        //                         f   d   u   s   m   c  e  t
        t4_evals[i] = t4.GetValue(tfc, tfc, tfc, tfc, tfc, tfc, i, tfc);
        std::cout << t4_evals.at(i) << std::endl;
    }

    return 0;
}
