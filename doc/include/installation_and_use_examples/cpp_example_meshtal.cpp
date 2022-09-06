#include "mcnptools/McnpTools.hpp"
#include <iomanip>
#include <iostream>
#include <vector>

int main() {

    // construct the meshtal class from meshtal file
    // "example_meshtal.mcnp.meshtal"
    mcnptools::Meshtal m("example_meshtal.mcnp.meshtal");

    // get tally 4 from the meshtal file
    mcnptools::MeshtalTally t4 = m.GetTally(4);

    // get the x and y bin centers
    std::vector<double> x = t4.GetXRBins();
    std::vector<double> y = t4.GetYZBins();

    // loop over x and y bins indices and print the tally value for
    // z index of 5
    std::cout << std::scientific << std::setprecision(5);
    for (unsigned int i = 0; i < x.size(); i++) {
        for (unsigned int j = 0; j < y.size(); j++) {
            std::cout << std::setw(12) << t4.GetValue(i, j, 5);
        }
        std::cout << std::endl;
    }

    return 0;
}
