from mcnptools import Ptrac
from sys import stdout

# explicitly open the file as a binary ptrac
p = Ptrac("example_ptrac_1.mcnp.ptrac", Ptrac.BIN_PTRAC)

# initialize counter
cnt = 0

# read histories in batches of 10000
hists = p.ReadHistories(10000)
while hists:

    # loop over all histories
    for h in hists:
        # loop over all events in the history
        for e in range(h.GetNumEvents()):

            event = h.GetEvent(e)

            if event.Type() == Ptrac.BNK:
                cnt += 1

                stdout.write(
                    "{:13d}{:13.5e}{:13.5e}{:13.5e}{:13.5e}\n".format(
                        cnt,
                        event.Get(Ptrac.X),
                        event.Get(Ptrac.Y),
                        event.Get(Ptrac.Z),
                        event.Get(Ptrac.ENERGY),
                    )
                )

    hists = p.ReadHistories(10000)
