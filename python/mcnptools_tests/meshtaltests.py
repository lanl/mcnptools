import unittest
from math import sqrt
from mcnptools import Meshtal, MeshtalTally 

test_meshtal = "../../libmcnptools/tests/test_files/meshtal"

class TestMeshtal(unittest.TestCase):
  def testMeshtalCode(self):
    msht = Meshtal(test_meshtal)
    code = msht.GetCode()
    check = "mcnp"
    self.assertEqual(check, code)

  def testMeshtalVersion(self):
    msht = Meshtal(test_meshtal)
    version = msht.GetVersion()
    check = "6"
    self.assertEqual(check,version)

  def testMeshtalComment(self):
    msht = Meshtal(test_meshtal)
    comment = msht.GetComment()
    check = "Input file to generate Mctal, Meshtal, and Ptrac files for MCNPTools testing"
    self.assertEqual(check,comment)

  def testMeshtalNps(self):
    msht = Meshtal(test_meshtal)
    nps = msht.GetNps()
    check = 225000
    self.assertEqual(check,nps)

  def testMeshtalWrite(self):
    meshtal = Meshtal(test_meshtal)
    
    # open a file and write the meshtal
    out_meshtal = open("written_meshtal", "w")
    out_meshtal.write( meshtal.WriteHeader() )
    meshtal_tallies = meshtal.GetTallyList()
    for tal in meshtal_tallies:
      tally = meshtal.GetTally(tal)
      out_meshtal.write("\n")
      out_meshtal.write( tally.WriteToMeshtal() )
    out_meshtal.close()

    # now read in the meshtal we just wrote
    written_meshtal = Meshtal("written_meshtal")
    written_meshtal_tallies = written_meshtal.GetTallyList()
    for i in range(len(meshtal_tallies)): self.assertEqual( written_meshtal_tallies[i], meshtal_tallies[i] )

    # compare individual tallies
    for tal in meshtal_tallies:
      tally = meshtal.GetTally(tal)
      written_tally = written_meshtal.GetTally(tal)

      # compare dimensions
      ibounds = tally.GetXRBounds()
      written_ibounds = written_tally.GetXRBounds()
      self.assertEqual( len(written_ibounds), len(ibounds) )
      for i in range(len(ibounds)): self.assertEqual( written_ibounds[i], ibounds[i] )

      jbounds = tally.GetYZBounds()
      written_jbounds = written_tally.GetYZBounds()
      self.assertEqual( len(written_jbounds), len(jbounds) )
      for i in range(len(jbounds)): self.assertEqual( written_jbounds[i], jbounds[i] )

      kbounds = tally.GetZTBounds()
      written_kbounds = written_tally.GetZTBounds()
      self.assertEqual( len(written_kbounds), len(kbounds) )
      for i in range(len(kbounds)): self.assertEqual( written_kbounds[i], kbounds[i] )

      ebounds = tally.GetEBounds()
      written_ebounds = written_tally.GetEBounds()
      self.assertEqual( len(written_ebounds), len(ebounds) )
      for i in range(len(ebounds)): self.assertEqual( written_ebounds[i], ebounds[i] )

      tbounds = tally.GetTBounds()
      written_tbounds = written_tally.GetTBounds()
      self.assertEqual( len(written_tbounds), len(tbounds) )
      for i in range(len(tbounds)): self.assertEqual( written_tbounds[i], tbounds[i] )

      ibins = tally.GetXRBins()
      written_ibins = written_tally.GetXRBins()
      self.assertEqual( len(written_ibins), len(ibins) )
      for i in range(len(ibins)): self.assertEqual( written_ibins[i], ibins[i] )

      jbins = tally.GetYZBins()
      written_jbins = written_tally.GetYZBins()
      self.assertEqual( len(written_jbins), len(jbins) )
      for i in range(len(jbins)): self.assertEqual( written_jbins[i], jbins[i] )

      kbins = tally.GetZTBins()
      written_kbins = written_tally.GetZTBins()
      self.assertEqual( len(written_kbins), len(kbins) )
      for i in range(len(kbins)): self.assertEqual( written_kbins[i], kbins[i] )

      ebins = tally.GetEBins()
      written_ebins = written_tally.GetEBins()
      self.assertEqual( len(written_ebins), len(ebins) )
      for i in range(len(ebins)): self.assertEqual( written_ebins[i], ebins[i] )

      tbins = tally.GetTBins()
      written_tbins = written_tally.GetTBins()
      self.assertEqual( len(written_tbins), len(tbins) )
      for i in range(len(tbins)): self.assertEqual( written_tbins[i], tbins[i] )

      # determine range of time/energy bins to check
      tstart = 0
      tlast = len(tbounds) - 1
      if len(tbounds) == 2:
        if tbounds[0] == -1e36 and tbounds[1] == 1e36:
          tlast = 1

      estart = 0
      elast = len(ebounds) - 1
      if len(ebounds) == 2:
        if ebounds[1] == 1e36:
          elast = 1

      # check values and errors
      for t in range(tstart, tlast):
        for e in range(estart, elast):
          for k in range(len(kbounds)-1):
            for j in range(len(jbounds)-1):
              for i in range(len(ibounds)-1):
                value = tally.GetValue(i, j, k, e, t)
                written_value = written_tally.GetValue(i,j,k,e,t)
                self.assertEqual(written_value, value)

                error = tally.GetError(i, j, k, e, t)
                written_error = written_tally.GetError(i,j,k,e,t)
                self.assertEqual(written_error, error)

class TestMeshtalTally(unittest.TestCase):
  def testMeshtalTallyID(self):
    msht = Meshtal(test_meshtal)

    tally = msht.GetTally(14)
    id = tally.ID()
    check = 14
    self.assertEqual(check,id)

    tally = msht.GetTally(54)
    id = tally.ID()
    check = 54
    self.assertEqual(check,id)

  def testMeshtalTallyBounds(self):
    def check_xyz_tallies( xyz_tallies ) :
      cbounds = [-10.00, -6.00, -2.00, 2.00, 6.00, 10.00]
      cbins = [-8.00, -4.00,  0.00, 4.00, 8.00]

      for tal in xyz_tallies:
        # x direction
        bounds = tal.GetXRBounds()
        bins = tal.GetXRBins()

        self.assertEqual( len(bounds), len(cbounds) )
        self.assertEqual( len(bins), len(cbins) )

        for i in range(len(bounds)): self.assertEqual(bounds[i], cbounds[i])
        for i in range(len(bins)): self.assertEqual(bins[i], cbins[i])

        # y direction
        bounds = tal.GetYZBounds()
        bins = tal.GetYZBins()

        self.assertEqual( len(bounds), len(cbounds) )
        self.assertEqual( len(bins), len(cbins) )

        for i in range(len(bounds)): self.assertEqual(bounds[i], cbounds[i])
        for i in range(len(bins)): self.assertEqual(bins[i], cbins[i])

        # z direction
        bounds = tal.GetZTBounds()
        bins = tal.GetZTBins()

        self.assertEqual( len(bounds), len(cbounds) )
        self.assertEqual( len(bins), len(cbins) )

        for i in range(len(bounds)): self.assertEqual(bounds[i], cbounds[i])
        for i in range(len(bins)): self.assertEqual(bins[i], cbins[i])

    def check_rzt_tallies( rzt_tallies ) :
      rbounds = [0.00,  2.50,  5.00,  7.50, 10.00]
      zbounds = [0.00,  2.00,  4.00,  6.00,  8.00, 10.00]
      tbounds = [0.000, 0.250, 0.500, 0.750, 1.000]
      rbins = [1.25,  3.75,  6.25,  8.75]
      zbins = [1.00,  3.00,  5.00,  7.00, 9.00]
      tbins = [0.125, 0.375, 0.625, 0.875]

      for tal in rzt_tallies:
        # r direction
        bounds = tal.GetXRBounds()
        bins = tal.GetXRBins()

        self.assertEqual( len(bounds), len(rbounds) )
        self.assertEqual( len(bins), len(rbins) )

        for i in range(len(bounds)): self.assertEqual(bounds[i], rbounds[i])
        for i in range(len(bins)): self.assertEqual(bins[i], rbins[i])

        # z direction
        bounds = tal.GetYZBounds()
        bins = tal.GetYZBins()

        self.assertEqual( len(bounds), len(zbounds) )
        self.assertEqual( len(bins), len(zbins) )

        for i in range(len(bounds)): self.assertEqual(bounds[i], zbounds[i])
        for i in range(len(bins)): self.assertEqual(bins[i], zbins[i])

        # z direction
        bounds = tal.GetZTBounds()
        bins = tal.GetZTBins()

        self.assertEqual( len(bounds), len(tbounds) )
        self.assertEqual( len(bins), len(tbins) )

        for i in range(len(bounds)): self.assertEqual(bounds[i], tbounds[i])
        for i in range(len(bins)): self.assertEqual(bins[i], tbins[i])

    def check_e_tallies( e_tallies ) :
      ebounds = [0.00E+00, 1.00E-09, 1.00E-03, 1.00E+03]
      ebins = [1.00E-09, 1.00E-03, 1.00E+03]

      for tal in e_tallies:
        bounds = tal.GetEBounds()
        bins = tal.GetEBins()

        self.assertEqual( len(bounds), len(ebounds) )
        self.assertEqual( len(bins), len(ebins) )

        for i in range(len(bounds)): self.assertEqual(bounds[i], ebounds[i])
        for i in range(len(bins)): self.assertEqual(bins[i], ebins[i])

    def check_t_tallies( t_tallies ) :
      tbounds = [-1.00E+36, 0.00E+00, 1.00E+01, 2.00E+01]
      tbins = [0.00E+00, 1.00E+01, 2.00E+01]

      for tal in t_tallies:
        bounds = tal.GetTBounds()
        bins = tal.GetTBins()

        self.assertEqual( len(bounds), len(tbounds) )
        self.assertEqual( len(bins), len(tbins) )

        for i in range(len(bounds)): self.assertEqual(bounds[i], tbounds[i])
        for i in range(len(bins)): self.assertEqual(bins[i], tbins[i])

    msht = Meshtal(test_meshtal)

    tally_14 = msht.GetTally(14)
    tally_24 = msht.GetTally(24)
    tally_34 = msht.GetTally(34)
    tally_44 = msht.GetTally(44)
    
    tally_54 = msht.GetTally(54)
    tally_64 = msht.GetTally(64)
    tally_74 = msht.GetTally(74)
    tally_84 = msht.GetTally(84)

    check_xyz_tallies( [tally_14, tally_24, tally_34, tally_44] )
    check_rzt_tallies( [tally_54, tally_64, tally_74, tally_84] )
    check_e_tallies( [tally_24, tally_44, tally_64, tally_84] )
    check_t_tallies( [tally_34, tally_44, tally_74, tally_84] )
    
  def testMeshtalTallyValue(self):
    msht = Meshtal(test_meshtal)
    tally_14 = msht.GetTally(14)
    tally_24 = msht.GetTally(24)
    tally_34 = msht.GetTally(34)
    tally_44 = msht.GetTally(44)

    # tally 14
    value = tally_14.GetValue(1,1,1);
    self.assertEqual(value, 3.00926E-03);

    # tally 24
    value = tally_24.GetValue(1,1,1); # total e bin
    self.assertEqual(value, 3.00926E-03);
    value = tally_24.GetValue(1,1,1,3); # total e bin
    self.assertEqual(value, 3.00926E-03);

    value = tally_24.GetValue(1,1,1,1);
    self.assertEqual(value, 9.72464E-04);

    # tally 34
    value = tally_34.GetValue(1,1,1);
    self.assertEqual(value, 2.24113E-03); # total t bin
    value = tally_34.GetValue(1,1,1,0,3);
    self.assertEqual(value, 2.24113E-03); # total t bin

    value = tally_34.GetValue(1,1,1,0,1);
    self.assertEqual(value, 2.15530E-03);

    # tally 44
    value = tally_44.GetValue(1,1,1);
    self.assertEqual(value, 2.24113E-03); # total e&t bin

    value = tally_44.GetValue(1,1,1,1);
    self.assertEqual(value, 2.04732E-04); # total t bin

    value = tally_44.GetValue(1,1,1,-1,1);
    self.assertEqual(value, 2.15530E-03); # total e bin

    value = tally_44.GetValue(1,1,1,1,1);
    self.assertEqual(value, 1.20977E-04); # total e bin

  def testMeshtalTallyError(self):
    msht = Meshtal(test_meshtal)
    tally_14 = msht.GetTally(14)
    tally_24 = msht.GetTally(24)
    tally_34 = msht.GetTally(34)
    tally_44 = msht.GetTally(44)

    # tally 14
    error = tally_14.GetError(1,1,1);
    self.assertEqual(error, 1.01983E-02);

    # tally 24
    error = tally_24.GetError(1,1,1); # total e bin
    self.assertEqual(error, 1.01983E-02);
    error = tally_24.GetError(1,1,1,3); # total e bin
    self.assertEqual(error, 1.01983E-02);

    error = tally_24.GetError(1,1,1,1);
    self.assertEqual(error, 1.91767E-02);

    # tally 34
    error = tally_34.GetError(1,1,1);
    self.assertEqual(error, 1.01749E-02); # total t bin
    error = tally_34.GetError(1,1,1,0,3);
    self.assertEqual(error, 1.01749E-02); # total t bin

    error = tally_34.GetError(1,1,1,0,1);
    self.assertEqual(error, 1.02654E-02);

    # tally 44
    error = tally_44.GetError(1,1,1);
    self.assertEqual(error, 1.01749E-02); # total e&t bin

    error = tally_44.GetError(1,1,1,1);
    self.assertEqual(error, 2.26073E-02); # total t bin

    error = tally_44.GetError(1,1,1,-1,1);
    self.assertEqual(error, 1.02654E-02); # total e bin

    error = tally_44.GetError(1,1,1,1,1);
    self.assertEqual(error, 2.44883E-02); # total e bin

  def testMeshtalTallyVolume(self):
    msht = Meshtal(test_meshtal)
    tally_14 = msht.GetTally(14)
    tally_24 = msht.GetTally(24)
    tally_34 = msht.GetTally(34)
    tally_44 = msht.GetTally(44)
    tally_54 = msht.GetTally(54)
    tally_64 = msht.GetTally(64)
    tally_74 = msht.GetTally(74)
    tally_84 = msht.GetTally(84)

    self.assertAlmostEqual(tally_14.GetVolume(2,2,2), tally_44.GetVolume(2,2,2), delta=1e-6);
    self.assertAlmostEqual(tally_24.GetVolume(2,2,2), tally_44.GetVolume(2,2,2), delta=1e-6);
    self.assertAlmostEqual(tally_34.GetVolume(2,2,2), tally_44.GetVolume(2,2,2), delta=1e-6);

    self.assertAlmostEqual(tally_54.GetVolume(1,1,1), tally_84.GetVolume(1,1,1), delta=5e-5);
    self.assertAlmostEqual(tally_64.GetVolume(1,1,1), tally_84.GetVolume(1,1,1), delta=5e-5);
    self.assertAlmostEqual(tally_74.GetVolume(1,1,1), tally_84.GetVolume(1,1,1), delta=5e-5);

  def testMeshtalStatisticalCombination(self):
    msht = Meshtal(test_meshtal)
    tally = msht.GetTally(14)
    nps = msht.GetNps()

    value = tally.GetValue(2,2,2)
    error = tally.GetError(2,2,2)

    m1 = value * nps
    m2 = nps * value**2 * (error**2*(nps-1)+1)

    tally &= tally

    value_new = (2*m1) / (2*nps)
    error_new = sqrt(((2*m2)/(2*nps) - value_new**2)/(2*nps-1) )/value_new

    self.assertAlmostEqual(value, value_new, delta=1e-6)
    self.assertAlmostEqual(error/sqrt(2), error_new, delta=1e-6)

