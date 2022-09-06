import unittest
from mcnptools import Ptrac, PtracHistory, PtracEvent

test_ptrac = "../../libmcnptools/tests/test_files/ptrac_asc"

class TestBinPtrac(unittest.TestCase):
  def testBinPtracReadHistories(self):
    ptrac = Ptrac(test_ptrac, Ptrac.ASC_PTRAC)

    hists = ptrac.ReadHistories(73)
    check = 73
    self.assertEqual(check, len(hists))

    hists = ptrac.ReadHistories(739812)
    check = 591 - 73
    self.assertEqual(check, len(hists))

    hists = ptrac.ReadHistories(872163)
    check = 0
    self.assertEqual(check, len(hists))

class TestBinPtracHistory(unittest.TestCase):
  def testBinPtracHistoryNps(self):
    ptrac = Ptrac(test_ptrac, Ptrac.ASC_PTRAC)
    hists = ptrac.ReadHistories(34)
    nps = hists[33].GetNPS().NPS()
    check = 34
    self.assertEqual(check,nps)

  def testBinPtracHistoryNumEvents(self):
    ptrac = Ptrac(test_ptrac, Ptrac.ASC_PTRAC)
    hists = ptrac.ReadHistories(1)
    nevents = hists[0].GetNumEvents()
    check = 14
    self.assertEqual(check,nevents)

class TestBinPtracEvent(unittest.TestCase):
  def testBinPtracEventType(self):
    ptrac = Ptrac(test_ptrac, Ptrac.ASC_PTRAC)
    hists = ptrac.ReadHistories(1)

    events_ok = True

    types = [ Ptrac.SRC,
              Ptrac.BNK,
              Ptrac.SUR,
              Ptrac.COL,
              Ptrac.TER ]

    for i in range(hists[0].GetNumEvents()):
      e = hists[0].GetEvent(i)

      event_type_found = False
      for j in range(5):
        if e.Type() == types[j]:
          event_type_found = True
          break

      if not event_type_found:
        events_ok = False
        break

    self.assertTrue(events_ok)

  def testBinPtracEventPositions(self):
    ptrac = Ptrac(test_ptrac, Ptrac.ASC_PTRAC)
    hists = ptrac.ReadHistories(1)

    e = hists[0].GetEvent(0)

    pos = [ e.Get(Ptrac.X), e.Get(Ptrac.Y), e.Get(Ptrac.Z) ];

    check = [ 0.0, 0.0, 0.0 ]

    for i in range(len(pos)): self.assertAlmostEqual(check[i],pos[i])

  def testBinPtracEventEnergy(self):
    ptrac = Ptrac(test_ptrac, Ptrac.ASC_PTRAC)
    hists = ptrac.ReadHistories(1)

    e = hists[0].GetEvent(0)
    
    energy = e.Get(Ptrac.ENERGY)

    check = 2.0

    self.assertAlmostEqual(check,energy)
    
  def testBinPtracEventTime(self):
    ptrac = Ptrac(test_ptrac, Ptrac.ASC_PTRAC)
    hists = ptrac.ReadHistories(1)

    e = hists[0].GetEvent(0)
    
    time = e.Get(Ptrac.TIME)

    check = 0.0

    self.assertAlmostEqual(check,time)
    
