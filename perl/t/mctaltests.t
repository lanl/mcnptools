use Test::More tests => 25;
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

sub check_close {
  my ($val1, $val2, $tol) = @_;

  my $pass = 1;
  if( abs($val1-$val2) > $tol ) {
    $pass = 0;
  }

  return $pass;
}

# MCTAL TESTS

my $test_mctal = "../libmcnptools/tests/mctal/test_mctal";

# MctalCode
{
  my $mctal = new McnpTools::Mctal($test_mctal);

  my $code = $mctal->GetCode();

  my $check = "mcnp";

  ok($check eq $code, "MctalCode");
}

# MctalDump
{
  my $mctal = new McnpTools::Mctal($test_mctal);

  my $dump = $mctal->GetDump();

  my $check = 2;

  ok($check == $dump, "MctalDump");
}

# MctalNps
{
  my $mctal = new McnpTools::Mctal($test_mctal);

  my $nps = $mctal->GetNps();

  my $check = 199990;

  ok($check == $nps, "MctalNps");
}

# MctalRandoms
{
  my $mctal = new McnpTools::Mctal($test_mctal);

  my $randoms = $mctal->GetRandoms();

  my $check = 5650622;

  ok($check == $randoms, "MctalRandoms");
}

# MctalComment
{
  my $mctal = new McnpTools::Mctal($test_mctal);

  my $comment = $mctal->GetComment();

  my $check = " mcnptools test input                                                           ";

  ok($check eq $comment, "MctalComment");
}

# MctalTallyList
{
  my $mctal = new McnpTools::Mctal($test_mctal);

  my $tallies = $mctal->GetTallyList();

  my $check = [4,14];

  ok(comp_list($tallies, $check, scalar(@{$tallies}), 0.0), "MctalTallyList");
}

# MCTAL TALLY TESTS

# MctalTallyID
{
  my $mctal = new McnpTools::Mctal($test_mctal);
  my $tally = $mctal->GetTally(4);

  my $id = $tally->ID();

  my $check = 4;

  ok($check == $id, "MctalTallyID");
}

# MctalTallyTFCBins
{
  my $mctal = new McnpTools::Mctal($test_mctal);
  my $tally = $mctal->GetTally(4);

  my $tfc = $tally->GetTFCBins();

  my $check = [ 0, 0, 0, 0, 0, 0, 5, 0 ];

  ok(comp_list($check, $tfc, 8, 0.0), "MctalTallyTFCBins");
}

# MctalTallyTFC
{
  my $mctal = new McnpTools::Mctal($test_mctal);
  my $tally = $mctal->GetTally(4);

  my $ntfc = $tally->GetTFCSize();

  ok($ntfc == 1, "MctalTallyTFCSize");

  # check function with default args
  ok($tally->GetTFCValue($McnpTools::MctalTally::TFC_NPS) == 199990, "MctalTallyTFC_NPS");
  ok($tally->GetTFCValue($McnpTools::MctalTally::TFC_VALUE) == 6.95380E-03 , "MctalTallyTFC_VALUE");
  ok($tally->GetTFCValue($McnpTools::MctalTally::TFC_ERROR) == 1.38355E-03, "MctalTallyTFC_ERROR");
  ok($tally->GetTFCValue($McnpTools::MctalTally::TFC_FOM) == 1.11444E+07, "MctalTallyTFC_FOM");

  # check function with specified tfc entry
  ok($tally->GetTFCValue($McnpTools::MctalTally::TFC_NPS,0) == 199990, "MctalTallyTFC_NPS");
  ok($tally->GetTFCValue($McnpTools::MctalTally::TFC_VALUE,0) == 6.95380E-03, "MctalTallyTFC_VALUE");
  ok($tally->GetTFCValue($McnpTools::MctalTally::TFC_ERROR,0) == 1.38355E-03, "MctalTallyTFC_ERROR");
  ok($tally->GetTFCValue($McnpTools::MctalTally::TFC_FOM,0) == 1.11444E+07, "MctalTallyTFC_FOM");
}

# MctalTallyFBins
{
  my $mctal = new McnpTools::Mctal($test_mctal);
  my $tally = $mctal->GetTally(4);

  my $fbins = $tally->GetFBins();

  my $check = [ 1000 ];

  ok(comp_list($check, $fbins, scalar(@{$fbins}), 0.0), "MctalTallyFBins");
}

# MctalTallyEBins
{
  my $mctal = new McnpTools::Mctal($test_mctal);
  my $tally = $mctal->GetTally(4);

  my $ebins = $tally->GetEBins();

  my $check = [ 1.00000E-03, 1.00000E-02, 1.00000E-01, 1.00000E+00, 1.00000E+01];

  ok(comp_list($check, $ebins, scalar(@{$ebins}), 0.0), "MctalTallyEBins");
}

# MctalTallyValue
{
  my $mctal = new McnpTools::Mctal($test_mctal);
  my $tally = $mctal->GetTally(4);

  my $value = $tally->GetValue($McnpTools::MctalTally::TFC,
                               $McnpTools::MctalTally::TFC,
                               $McnpTools::MctalTally::TFC,
                               $McnpTools::MctalTally::TFC,
                               $McnpTools::MctalTally::TFC,
                               $McnpTools::MctalTally::TFC,
                               2,
                               $McnpTools::MctalTally::TFC );


  my $check = 1.60023E-04;

  ok($check == $value, "MctalTallyValue");
}
  
# MctalTallyError
{
  my $mctal = new McnpTools::Mctal($test_mctal);
  my $tally = $mctal->GetTally(4);

  my $error = $tally->GetError($McnpTools::MctalTally::TFC,
                               $McnpTools::MctalTally::TFC,
                               $McnpTools::MctalTally::TFC,
                               $McnpTools::MctalTally::TFC,
                               $McnpTools::MctalTally::TFC,
                               $McnpTools::MctalTally::TFC,
                               $McnpTools::MctalTally::TFC,
                               $McnpTools::MctalTally::TFC );


  my $check = 0.0014;

  ok($check == $error, "MctalTallyError");
}
  
# Mctal Kcode Tests

# MctalKcodeCycles
{
  my $mctal = new McnpTools::Mctal($test_mctal);
  my $kcode = $mctal->GetKcode();

  my $cycles = $kcode->GetCycles();

  my $check = 200;

  ok($check == $cycles, "MctalKcodeCycles");
} 

# MctalKcodeSettle
{
  my $mctal = new McnpTools::Mctal($test_mctal);
  my $kcode = $mctal->GetKcode();

  my $settle = $kcode->GetSettle();

  my $check = 20;

  ok($check == $settle, "MctalKcodeSettle");
} 

# MctalKcodeNdat
{
  my $mctal = new McnpTools::Mctal($test_mctal);
  my $kcode = $mctal->GetKcode();

  my $ndat = $kcode->GetNdat();

  my $check = 19;

  ok($check == $ndat, "MctalKcodeNdat");
} 

# MctalKcodeValue
{
  my $mctal = new McnpTools::Mctal($test_mctal);
  my $kcode = $mctal->GetKcode();

  my $keff = $kcode->GetValue($McnpTools::MctalKcode::AVG_COMBINED_KEFF);

  my $check = 1.00265E+00;

  ok($check == $keff, "MctalKcodeValue");
}
