use Test::Simple tests => 8;
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

# PTRAC TESTS

my $test_ptrac = "../libmcnptools/tests/ptrac/test_ptrac";

# PtracReadHistories
{
  my $ptrac = new McnpTools::Ptrac($test_ptrac);

  my $hists = $ptrac->ReadHistories(73);

  my $check = 73;

  ok($check == scalar(@{$hists}), "PtracReadHistories batch 1");

  $hists = $ptrac->ReadHistories(739812);

  $check = 185 - 73;

  ok($check == scalar(@{$hists}), "PtracReadHistories batch 2");

  $hists = $ptrac->ReadHistories(872163);

  $check = 0;

  ok($check == scalar(@{$hists}), "PtracReadHistories batch 3");
}

# PtracHistoryNps
{
  my $ptrac = new McnpTools::Ptrac($test_ptrac);
  my $hists = $ptrac->ReadHistories(34);

  my $nps = $hists->[33]->GetNPS()->NPS();
  my $check = 34;

  ok($check == $nps, "PtracHistoryNps");
}

# PtracHistoryNumEvents
{
  my $ptrac = new McnpTools::Ptrac($test_ptrac);
  my $hists = $ptrac->ReadHistories(1);
  
  my $nevent = $hists->[0]->GetNumEvents();

  my $check = 7;

  ok($check == $nevent, "PtracHistoryNumEvents");
}

# PtracEventType
{
  my $ptrac = new McnpTools::Ptrac($test_ptrac);
  my $hists = $ptrac->ReadHistories(1);

  my $events_ok = 1;

  my $types = [ $McnpTools::Ptrac::SRC,
                $McnpTools::Ptrac::BNK,
                $McnpTools::Ptrac::SUR,
                $McnpTools::Ptrac::COL,
                $McnpTools::Ptrac::TER ];

  for(my $i=0; $i < $hists->[0]->GetNumEvents(); $i++) {
    my $e = $hists->[0]->GetEvent($i);

    my $event_type_found = 0;
    for(my $j=0; $j<5; $j++) {
      if( $e->Type() == $types->[$j] ) {
        $event_type_found = 1;
        last;
      }
    }

    if( ! $event_type_found ) {
      $events_ok = 0;
      last;
    }
  }

  ok($events_ok, "PtracEventType");
}

# PtracEventEnergy
{
  my $ptrac = new McnpTools::Ptrac($test_ptrac);
  my $hists = $ptrac->ReadHistories(1);

  my $e = $hists->[0]->GetEvent(0);

  my $energy = $e->Get( $McnpTools::Ptrac::ENERGY );

  my $check = 2.0;

  ok($check == $energy, "PtracEventEnergy");
}

# PtracEventTime
{
  my $ptrac = new McnpTools::Ptrac($test_ptrac);
  my $hists = $ptrac->ReadHistories(1);

  my $e = $hists->[0]->GetEvent(0);

  my $time = $e->Get( $McnpTools::Ptrac::TIME );

  my $check = 0.0;

  ok($check == $time, "PtracEventTime");
}
