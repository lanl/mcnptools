#include "mcnptools/McnpTools.hpp"
#include <iomanip>
#include <iostream>
#include <vector>

int main() {

    std::cout << std::scientific << std::setprecision(5);

    // explicitly open the file as a binary ptrac
    mcnptools::Ptrac p("example_ptrac_1.mcnp.ptrac", mcnptools::Ptrac::BIN_PTRAC);

    // initialize counter
    unsigned int cnt = 0;

    // read histories in batches of 10000
    std::vector<mcnptools::PtracHistory> hists = p.ReadHistories(10000);
    while (hists.size() > 0) {

        // loop over all histories
        for (unsigned int h = 0; h < hists.size(); h++) {
            // loop over all events in the history
            for (unsigned int e = 0; e < hists.at(h).GetNumEvents(); e++) {

                mcnptools::PtracEvent event = hists.at(h).GetEvent(e);

                if (event.Type() == mcnptools::Ptrac::BNK) {
                    cnt += 1;
                    std::cout << std::setw(13) << cnt << std::setw(13)
                              << event.Get(mcnptools::Ptrac::X) << std::setw(13)
                              << event.Get(mcnptools::Ptrac::Y) << std::setw(13)
                              << event.Get(mcnptools::Ptrac::Z) << std::setw(13)
                              << event.Get(mcnptools::Ptrac::ENERGY) << std::endl;
                }
            }
        }
        hists = p.ReadHistories(10000);
    }
    return 0;
}
