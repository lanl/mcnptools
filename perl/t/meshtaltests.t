use Test::Simple tests => 14;
use McnpTools;
use strict;

sub comp_list {
  my ($lref1, $lref2, $size, $tol) = @_;
  
  my $pass = 1;
  if( scalar(@{$lref1}) != scalar(@{$lref2}) ) {
    $pass = 0;
  }
  else {
    for(my $i=0; $i<$size; $i++) {
      if( abs($lref1->[$i] - $lref2->[$i]) > $tol ) {
        $pass = 0;
      }
    }
  }

  return $pass;
}

# MESHTAL TESTS

my $test_meshtal = "../libmcnptools/tests/meshtal/test_meshtal";

# MeshtalCode
{
  my $msht = new McnpTools::Meshtal($test_meshtal);

  my $code = $msht->GetCode();

  my $check = "mcnp";

  ok($check eq $code, "MeshtalCode");
}

# MeshtalVersion
{
  my $msht = new McnpTools::Meshtal($test_meshtal);
  
  my $version = $msht->GetVersion();

  my $check = "6";

  ok($check eq $version, "MeshtalVersion")
}

# MeshtalComment
{
  my $msht = new McnpTools::Meshtal($test_meshtal);
  
  my $comment = $msht->GetComment();

  my $check = "mcnptools test input";

  ok($check eq $comment, "MeshtalComment")
}

# MeshtalNps
{
  my $msht = new McnpTools::Meshtal($test_meshtal);
  
  my $nps = $msht->GetNps();

  my $check = 180000;

  ok($check == $nps, "MeshtalNps")
}

# MeshtalTallyID
{
  my $msht = new McnpTools::Meshtal($test_meshtal);
  my $tally = $msht->GetTally(4);
  
  my $id = $tally->ID();

  my $check = 4;

  ok($check == $id, "MeshtalTallyID");
}

# MeshtalTallyBounds
{
  my $msht = new McnpTools::Meshtal($test_meshtal);
  my $tally = $msht->GetTally(4);

  my $ibounds = $tally->GetXRBounds();
  my $icheck1 = [-5.00, -4.00, -3.00, -2.00, -1.00, 0.00, 1.00, 2.00, 3.00, 4.00, 5.00];
  ok( comp_list($icheck1, $ibounds, scalar(@{$ibounds}), 1e-3), "MeshtalTallyBounds 1");

  my $ibins = $tally->GetXRBins();
  my $icheck2 = [-4.50, -3.50, -2.50, -1.50, -0.50, 0.50, 1.50, 2.50, 3.50, 4.50];
  ok( comp_list($icheck2, $ibins, scalar(@{$ibins}), 1e-3), "MeshtalTallyBounds 2");

  my $jbounds = $tally->GetYZBounds();
  my $jcheck1 = [-5.00, -4.00, -3.00, -2.00, -1.00, 0.00, 1.00, 2.00, 3.00, 4.00, 5.00];
  ok( comp_list($jcheck1, $jbounds, scalar(@{$jbounds}), 1e-3), "MeshtalTallyBounds 3");

  my $jbins = $tally->GetYZBins();
  my $jcheck2 = [-4.50, -3.50, -2.50, -1.50, -0.50, 0.50, 1.50, 2.50, 3.50, 4.50];
  ok( comp_list($jcheck2, $jbins, scalar(@{$jbins}), 1e-3), "MeshtalTallyBounds 4");

  my $kbounds = $tally->GetZTBounds();
  my $kcheck1 = [-5.00, -4.00, -3.00, -2.00, -1.00, 0.00, 1.00, 2.00, 3.00, 4.00, 5.00];
  ok( comp_list($kcheck1, $kbounds, scalar(@{$kbounds}), 1e-3), "MeshtalTallyBounds 5");

  my $kbins = $tally->GetZTBins();
  my $kcheck2 = [-4.50, -3.50, -2.50, -1.50, -0.50, 0.50, 1.50, 2.50, 3.50, 4.50];
  ok( comp_list($kcheck2, $kbins, scalar(@{$kbins}), 1e-3), "MeshtalTallyBounds 6");
}

# MeshtalTallyValue
{
  my $msht = new McnpTools::Meshtal($test_meshtal);
  my $tally = $msht->GetTally(4);

  my $value = $tally->GetValue(5,5,5);

  my $check = 1.28113E-02;

  ok(abs($check - $value) < 1e-6, "MeshtalTallyValue\n$value\n$check");
}

# MeshtalTallyError
{
  my $msht = new McnpTools::Meshtal($test_meshtal);
  my $tally = $msht->GetTally(4);

  my $error = $tally->GetError(5,5,5);

  my $check = 1.77364E-02;

  ok(abs($check - $error) < 1e-6, "MeshtalTallyError");
}

# MeshtalTallyVolume
{
  my $msht = new McnpTools::Meshtal($test_meshtal);
  my $tally = $msht->GetTally(4);

  my $volume = $tally->GetVolume(5,5,5);

  my $check = 1.00000E+00;

  ok(abs($check - $volume) < 1e-6, "MeshtalTallyVolume");
}
