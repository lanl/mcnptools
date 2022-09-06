from mcnptools import Ptrac
from sys import stdout

# explicitly open the file as a legacy binary ptrac
p = Ptrac("example_ptrac_2.mcnp.ptrac", Ptrac.BIN_PTRAC)

# read histories in batches of 10000
hists = p.ReadHistories(10000)

while hists:

    # loop over all histories
    for h in hists:
        # loop over all events in the history
        for e in range(h.GetNumEvents()):

            event = h.GetEvent(e)

            if event.Type() == Ptrac.SUR:
                stdout.write(
                    "{:13.5e}{:13.5e}{:13.5e}{:13.5e}\n".format(
                        event.Get(Ptrac.X),
                        event.Get(Ptrac.Y),
                        event.Get(Ptrac.Z),
                        event.Get(Ptrac.ANGLE),
                    )
                )

    hists = p.ReadHistories(10000)
