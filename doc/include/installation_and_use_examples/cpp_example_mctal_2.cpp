#include "mcnptools/McnpTools.hpp"
#include <iostream>

int main() {

    // construct the mctal class from the mctal file
    // "example_mctal_2.mcnp.mctal"
    mcnptools::Mctal m("example_mctal_2.mcnp.mctal");

    // get the kcode data
    mcnptools::MctalKcode kc = m.GetKcode();

    // alias for average combined keff
    unsigned int keff = mcnptools::MctalKcode::AVG_COMBINED_KEFF;
    // alias for average combined keff standard deviation
    unsigned int keff_std = mcnptools::MctalKcode::AVG_COMBINED_KEFF_STD;

    // loop over ACTIVE cycles and print
    for (unsigned int i = kc.GetSettle(); i < kc.GetCycles(); i++) {
        std::cout << i << "  " << kc.GetValue(keff, i) << "  " << kc.GetValue(keff_std, i)
                  << std::endl;
    }

    return 0;
}
