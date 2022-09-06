from mcnptools import Ptrac
from sys import stdout

# explicitly open the file as an HDF5 ptrack
p = Ptrac("example_ptrac_3.mcnp.ptrac.h5", Ptrac.HDF5_PTRAC)

# read histories in batches of 10000
hists = p.ReadHistories(10000)

while hists:

    # loop over all histories
    for h in hists:
        print("History: ", h.GetNPS().NPS())
        # loop over all events in the history
        for e in range(h.GetNumEvents()):

            event = h.GetEvent(e)

            if event.Type() == Ptrac.SUR:
                stdout.write(
                    "SUR: {:13.5e}{:13.5e}{:13.5e}{:13.5e}\n".format(
                        event.Get(Ptrac.X),
                        event.Get(Ptrac.Y),
                        event.Get(Ptrac.Z),
                        event.Get(Ptrac.ANGLE),
                    )
                )
            elif event.Type() == Ptrac.TER:
                stdout.write(
                    "TER: {:13.5e}{:13.5e}{:13.5e}{:13.5e}\n".format(
                        event.Get(Ptrac.X),
                        event.Get(Ptrac.Y),
                        event.Get(Ptrac.Z),
                        event.Get(Ptrac.TERMINATION_TYPE),
                    )
                )

    hists = p.ReadHistories(10000)
