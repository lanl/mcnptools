import unittest
from mcnptools import Mctal, MctalTally, MctalKcode

test_mctal = "../../libmcnptools/tests/test_files/mctal"

class TestMctal(unittest.TestCase):
  def testMctalCode(self):
    mctal = Mctal(test_mctal)
    code = mctal.GetCode()
    check = "mcnp"
    self.assertEqual(check,code)

  def testMctalDump(self):
    mctal = Mctal(test_mctal)
    dump = mctal.GetDump()
    check = 2
    self.assertEqual(check,dump)
    
  def testMctalNps(self):
    mctal = Mctal(test_mctal)
    nps = mctal.GetNps()
    check = 248943
    self.assertEqual(check,nps)

  def testMctalRandoms(self):
    mctal = Mctal(test_mctal)
    randoms = mctal.GetRandoms()
    check = 62983502
    self.assertEqual(check,randoms)

  def testMctalComment(self):
    mctal = Mctal(test_mctal)
    comment = mctal.GetComment()
    check = " Input file to generate Mctal, Meshtal, and Ptrac files for MCNPTools testing   "
    self.assertEqual(check,comment)

  def testMctalTallyList(self):
    mctal = Mctal(test_mctal)
    tallies = mctal.GetTallyList()
    check = [1,4]
    for i in range(len(tallies)): self.assertEqual(check[i],tallies[i]) 

class TestMctalTally(unittest.TestCase):
  def testMctalTallyId(self):
    mctal = Mctal(test_mctal)
    tally = mctal.GetTally(4)
    id = tally.ID()
    check = 4
    self.assertEqual(check,id)

  def testMctalTallyTFCBins(self):
    mctal = Mctal(test_mctal)
    tally = mctal.GetTally(1)
    tfc = tally.GetTFCBins()

    self.assertEqual( len(tfc), 8 )

    check = [ 0, 0, 0, 0, 0, 1, 3, 3 ];
    for i in range(len(tfc)): self.assertEqual(check[i],tfc[i]) 

  def testMctalTallyTFC(self):
    mctal = Mctal(test_mctal)
    tally = mctal.GetTally(1)
    self.assertEqual(tally.GetTFCSize(),1)

    # check function with default args
    self.assertAlmostEqual(tally.GetTFCValue(tally.TFC_NPS), 248943)
    self.assertAlmostEqual(tally.GetTFCValue(tally.TFC_VALUE), 8.34542E-01)
    self.assertAlmostEqual(tally.GetTFCValue(tally.TFC_ERROR), 8.29886E-04)
    self.assertAlmostEqual(tally.GetTFCValue(tally.TFC_FOM), 5.09070E+06)

    # check function with specified tfc entry
    self.assertAlmostEqual(tally.GetTFCValue(tally.TFC_NPS,0), 248943)
    self.assertAlmostEqual(tally.GetTFCValue(tally.TFC_VALUE,0), 8.34542E-01)
    self.assertAlmostEqual(tally.GetTFCValue(tally.TFC_ERROR,0), 8.29886E-04)
    self.assertAlmostEqual(tally.GetTFCValue(tally.TFC_FOM,0), 5.09070E+06)

  def testMctalTallyFBins(self):
    mctal = Mctal(test_mctal)

    tally = mctal.GetTally(1)
    fbins = tally.GetFBins()
    self.assertEqual( len(fbins), 2 )
    check = [101, 102]
    for i in range(len(fbins)): self.assertEqual(check[i],fbins[i])

    tally = mctal.GetTally(4)
    fbins = tally.GetFBins()
    self.assertEqual( len(fbins), 2 )
    check = [1001, 1002]
    for i in range(len(fbins)): self.assertEqual(check[i],fbins[i])

  def testMctalTallyCBins(self):
    mctal = Mctal(test_mctal)
    check = [0.00000E+00, 1.00000E+00]

    tally = mctal.GetTally(1)
    cbins = tally.GetCBins()
    self.assertEqual( len(cbins), 2 )
    for i in range(len(cbins)): self.assertEqual(check[i],cbins[i])

    tally = mctal.GetTally(4)
    cbins = tally.GetCBins()
    self.assertEqual( len(cbins), 1 )

  def testMctalTallyEBins(self):
    mctal = Mctal(test_mctal)
    check = [ 1.00000E-09,  1.00000E-03,  1.00000E+03 ]

    tally = mctal.GetTally(1)
    ebins = tally.GetEBins()
    self.assertEqual( len(ebins), 3 )
    for i in range(len(ebins)): self.assertEqual(check[i],ebins[i])

    tally = mctal.GetTally(4)
    ebins = tally.GetEBins()
    self.assertEqual( len(ebins), 3 )
    for i in range(len(ebins)): self.assertEqual(check[i],ebins[i])

  def testMctalTallyTBins(self):
    mctal = Mctal(test_mctal)
    check = [ 0.00000E+00,  1.00000E+01,  2.00000E+01 ]

    tally = mctal.GetTally(1)
    tbins = tally.GetTBins()
    self.assertEqual( len(tbins), 3 )
    for i in range(len(tbins)): self.assertEqual(check[i],tbins[i])

    tally = mctal.GetTally(4)
    tbins = tally.GetTBins()
    self.assertEqual( len(tbins), 3 )
    for i in range(len(tbins)): self.assertEqual(check[i],tbins[i])

  def testMctalTallyValue(self):
    mctal = Mctal(test_mctal)
    tally = mctal.GetTally(1)
    value = tally.GetValue( MctalTally.TFC,
                            MctalTally.TFC,
                            MctalTally.TFC,
                            MctalTally.TFC,
                            MctalTally.TFC,
                            MctalTally.TFC,
                            2,
                            MctalTally.TFC )
    check = 8.32400E-01
    self.assertAlmostEqual(check,value)

  def testMctalTallyError(self):
    mctal = Mctal(test_mctal)
    tally = mctal.GetTally(4)
    error = tally.GetError( 1,
                            MctalTally.TFC,
                            MctalTally.TFC,
                            MctalTally.TFC,
                            MctalTally.TFC,
                            MctalTally.TFC,
                            2,
                            1)
    check = 0.0012;
    self.assertAlmostEqual(check,error)

class TestMctalKcode(unittest.TestCase):
  def testMctalKcodeCycles(self):
    mctal = Mctal(test_mctal)
    kcode = mctal.GetKcode()

    cycles = kcode.GetCycles()
    check = 50
    self.assertEqual(check,cycles)

    cycles = kcode.GetSettle()
    check = 5
    self.assertEqual(check, cycles)

  def testMctalKcodeNdat(self):
    mctal = Mctal(test_mctal)
    kcode = mctal.GetKcode()
    ndat = kcode.GetNdat()
    check = 19
    self.assertEqual(check,ndat)

  def testMctalKcodeValue(self):
    mctal = Mctal(test_mctal)
    kcode = mctal.GetKcode()

    keff = kcode.GetValue(MctalKcode.AVG_COMBINED_KEFF)
    check = 7.63582E-01
    self.assertEqual(check,keff)

    kstd = kcode.GetValue(MctalKcode.AVG_COMBINED_KEFF_STD)
    check = 1.72487E-03
    self.assertEqual(check,kstd)
