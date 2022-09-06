#!/usr/bin/perl
########################################################################
#
# fpp -- preprocessor for fortran files
#
# History:
#   1992-12-12  - fbb, initial version for ANL Vim code
#   2001-06-06  - fbb, initial version for MCNP5
#   2010-06-06  - fbb, mods for MCNP6
#   2014-01-07  - fbb, bug fix for pattern matching in arg processing 
#                      (for -I,-o,-s,-e) - must match start of pattern.
#   2014-01-15  - fbb, mods for templating
#.......................................................................
#
#  Handles these constructs:
#
#    #ifdef  DEF
#    #else
#    #endif
#
#    #ifndef DEF
#    #else
#    #endif
#
#    #define DEF
#
#    #undef DEF
#
#    #include "F"
#
#    #if defined(DEF1) op defined(DEF2) op ...
#    #else
#    #endif
#              where "op" is |, &, ||, &&
#              and "defined()" may be preceded with "!"
# 
#    Substitutions for variables defined   as -Dvar=value,
#     (only if macro substitution is on (-F or -macros),
#      on by default in MCNP5 build system, off for MCNP6.)
# 
#  These preprocessor constructs are ignored:
#    #include  <file>
#    #include  'file'
#    #include   file
#    #elif
#    #if     [anything other than "defined()...." ]
#
#  Long fpp preprocessor lines may be continued on successive lines by using a
#    backslash ("\") as the last character. The continuation lines should NOT
#    include a "#" character at their beginning.
#
#  Templating (added 2013-12-20, fbb)
#
#    #FPP_TEMPLATE_BEGIN
#    #FPP_TEMPLATE    <a> =  a1; a2; a3; ...; aN
#    #FPP_TEMPLATE    <b> =  b1; b2; b3; ...; bN
#
#    all coding here (after other fpp processing) is replicated N times, 
#        with substitutions for <a>, <b>, etc.
#    the keys, <a>, <b> etc., should be \w+ (alphanumeric, underscore) 
#        only, without blanks, and are case-sensitive. The <> delimiters 
#        are required.
#    the subsititutions are separated by semicolons, hence could be any
#        character string not containing semicolons. Do not include a 
#        trailing semicolon on the last entry in a list.
#    fpp template constructs cannot be nested within a single file, but
#        may contain other preprocessor constructs, or may be enclosed
#        within other constructs (eg, #ifdef's).
#
#    #FPP_TEMPLATE_END
#.......................................................................
# 
#  Usage:
#
#    fpp    [-Dopt] [-Dopt=val] [-Uopt] [-I file] [-o outfile]
#           [-F] [-macros] [-nomacros] [-blanks] [-noblanks]
#           [-s suffix] [-noblanks] [--]  files
#
#      * optional spaces or quotes allowed after -D, -U, -I, or -o
#
#      * reads from stdin if no files listed on cmd line 
#             ***not for Win32***
#
#      * -I specifies an include-directory, can be repeated
#        If include-file begins with "/" it is used, 
#        otherwise, check ".", then include-dirs (in cmdline order)
#
#      * For MCNP5,
#          MACRO EXPANSION SHOULD ALWAYS BE PERFORMED.
#          The default value for $macro_substitute should be 1.
#        ForMCNP6,
#          MACRO EXPANSION IN SOURCE CODING IS NOT PERFORMED
#          UNLESS THE -F OPTION IS SPECIFIED   (change: 11/9/2004)
#          The default value for $macro_substitute should be 0.
#
#       -macros     turns on  macro substitution, equivalent to -F
#       -nomacros   turns off macro substitution
#
#      * these options are ignored:  -P, -E, -eP
#
#      * if "-o file" not supplied, writes to stdout
#
#      * if "-s suffix" supplied, writes to file with suffix extension.
#        example:  -s .i90  puts output into  file.i90
#
#
#      * -noblanks  indicates that blank lines should NOT
#                   be output for # lines or deleted lines
#
#                   For MCNP5, -noblanks is the default,
#                              set $blanks=0
#                   For MCNP6, blank lines SHOULD be output,
#                              set $blanks=1
#
#                     Copyright LANL/LANS/DOE - see file COPYRIGHT_INFO
########################################################################
$verbose = 0;
#
# MCNP6 defaults:  -nomacros  -blanks
    $macro_substitute = 0;
    $blanks  = 1;
# MCNP5 defaults:  -macros  -noblanks
#   $macro_substitute = 1;
#   $blanks  = 0;
#
##### get args
#
push @INCS, '.';
while(   $arg = shift @ARGV ) {
  if(    $arg eq '-v'              ) { $verbose=1; }
  elsif( $arg eq '-D'              ) { $defs{ shift(@ARGV) } = 1; }
  elsif( $arg =~ /^-D(\S+)=(.+)$/  ) { $defs{$1} = 1; $subs{$1} = "$2"; }
  elsif( $arg =~ /^-D(\S+)$/       ) { $defs{$1} = 1; }
  elsif( $arg eq '-U'              ) { delete $defs{ shift(@ARGV) }; }
  elsif( $arg =~ /^-U(\S+)$/       ) { delete $defs{$1}; }
  elsif( $arg eq '-I'              ) { push @INCS, shift(@ARGV); }
  elsif( $arg =~ /^-I(\S+)/        ) { push @INCS, $1; }
  elsif( $arg eq '-o'              ) { $OUT = shift(@ARGV); }
  elsif( $arg =~ /^-o(\S+)/        ) { $OUT = $1; }
  elsif( $arg eq '-s'              ) { $EXT = shift(@ARGV); }
  elsif( $arg =~ /^-s(\S+)/        ) { $EXT = $1; }
  elsif( $arg eq '-F'              ) { $macro_substitute=1; }
  elsif( $arg eq '-macros'         ) { $macro_substitute=1; }
  elsif( $arg eq '-nomacros'       ) { $macro_substitute=0; }
  elsif( $arg eq '-noblanks'       ) { $blanks = 0;         }
  elsif( $arg eq '-blanks'         ) { $blanks = 1;         }
  elsif( $arg eq '-P'              ) { ; }
  elsif( $arg eq '-E'              ) { ; }
  elsif( $arg eq '-e'              ) { shift(@ARGV); }
  elsif( $arg =~ /^-e\S+/          ) { ; }
  elsif( $arg eq '--'              ) { last; }
  elsif( $arg eq '-pthread'        ) { ; } #problem with OpenMPI on Yellowrail 
  else                               { unshift(@ARGV,$arg); last; } 
}
if( !@ARGV ) { unshift(@ARGV,'-'); }
if( $verbose ) {
  foreach $k (keys(%defs)) {
    if( $defs{$k} ) {
      if( $subs{$k} )  { print STDERR " .......defined:  $k\n";}
      else             { print STDERR " .......defined:  $k= $subs{$k}\n";}
    }
    else               { print STDERR " ...not defined:  $k\n"; }
  }
  print STDERR " ....macro_subs= $macro_substitute\n";
  print STDERR " ........blanks= $blanks\n";
  print STDERR " ..include dirs: @INCS\n";
  print STDERR " ...output file: $OUT\n";
  print STDERR " .........files: @ARGV\n";
}
if( $OUT && $EXT ) { die("$0: can't use both -o and -s\n"); }
#
##### redirect output to file, stdout by default
#
if( $OUT ) {
  open( OUT, ">$OUT") || die("$0: can't open $OUT\n");
  *STDOUT = *OUT;
}
#
##### process each file from command line
#
foreach $F (@ARGV) {

  if( $EXT ) {
    $i = rindex($F,'.');
    if( $i==-1 ) { $G = $F.$EXT; }
    else         { $G = substr($F,0,$i).$EXT; }
    open( OUT, ">$G") || die("$0: can't open $G\n");
    *STDOUT = *OUT;
  }
  open( F,  "<$F" ) || die("$0: can't open $F\n");

  &process_file( *F, 1, 1, 0 );

  close(F);
  close(OUT) if $EXT;
}
close(OUT) if $OUT;
exit;
#
##### recursive sub, to process lines in current file
#
sub process_file {
  local( *F ) = $_[0];   ### current filehandle
  my( $ok   ) = $_[1];   ### t/f, outer condition for print
  my( $def  ) = $_[2];   ### t/f, active def, for current level
  my( $level) = $_[3];   ### int, current level in recursion
  my( $newok, $newdef, $G, $dir, $dstr );
  my( @template_name, @template_subs );
  my( @template_repl, @template_junk );
  my( $template_count,$template_copies, $n, $t );
  my( $ftplate_file   ) = "fpp_temp_file_for_template_$$-$level.txt";
  local( *G, *FTPLATE, *STDOUT_PREVIOUS );

  $newok = $ok && $def;
  $level++;
  
  while( <F> ) {

    # pick up continuation lines for #directives
    if( /^\s*#/ ) {
      while( substr($_,-2,1) eq "\\" ) { $_ = substr($_,0,-2).<F>; }
    }
    
    if( /^\s*#\s*include\s+"([^"]+)"/ ) {
      print "\n"   if $blanks;
      next unless $newok;
      $G = $1;
      if( $G =~ m(^[^/]) ) {
        foreach $dir (@INCS) {
          if( -r $dir.'/'.$G ) { $G = $dir.'/'.$G; last; }
        }
      }
      open( G, "<$G") || die("$0: can't open $G\n");
      &process_file( *G, $newok, $def, $level );
      close(G); 
    }
    elsif( /^\s*#\s*define\s+(\w+)\s*$/ ) {
      print "\n"   if $blanks;
      $defs{$1} = 1     if $newok;
    }
    elsif( /^\s*#\s*define\s+(\w+)\s+(.+)/ ) {
      print "\n"   if $blanks;
      $defs{$1} = "$2"  if $newok;
    }
    elsif( /^\s*#\s*undef\s+(\w+)/ ) {
      print "\n"   if $blanks;
      $defs{$1} = ''    if $newok;
      $subs{$1} = ''    if $newok;
    }
    elsif( /^\s*#\s*ifdef\s+(\w+)/ ) { 
      print "\n"   if $blanks;
      $newdef = $defs{$1};
      &process_file( *F, $newok, $newdef, $level );
    }
    elsif( /^\s*#\s*ifndef\s+(\w+)/ ) {
      print "\n"   if $blanks;
      $newdef = ! $defs{$1};
      &process_file( *F, $newok, $newdef, $level );
    }
    elsif( /^\s*#if\s+(!?\s*defined\(.*)/ ) {
      print "\n"   if $blanks;
      $dstr   = $1;
      $dstr   =~ s/defined\(\s*(\w+)\s*\)/\$defs\{\1\}/go;
      $newdef = eval $dstr;
      &process_file( *F, $newok, $newdef, $level ); 
    }
    elsif( /^\s*#if\s+\((!?\s*defined\(.*)\)/ ) {
      print "\n"   if $blanks;
      $dstr   = $1;
      $dstr   =~ s/defined\(\s*(\w+)\s*\)/\$defs\{\1\}/go;
      $newdef = eval $dstr;
      &process_file( *F, $newok, $newdef, $level ); 
    }
    elsif(  /^\s*#\s*endif/ ) {
      print "\n"   if $blanks;
      return;
    }
    elsif(  /^\s*#\s*else/  ) {
      print "\n"   if $blanks;
      $newok = $ok && ! $def;
    }
    elsif(  /^\s*#\s*include/  ) {
      # ignore #include statements lines
      #print "\n";
       my $temp = $_;
       chomp $temp;
      print STDERR "\nPreprocessor warning: ignoring stmt: $temp";
    }

    elsif(  /^\s*#\s*FPP_TEMPLATE_BEGIN/  ) {
      # switch stdout to scratch file
      *STDOUT_PREVIOUS = *STDOUT;
      open( FTPLATE, ">$ftplate_file" );
      *STDOUT = *FTPLATE;
      $template_count = 0;
      undef @template_name;
      undef @template_subs;
      undef @template_junk;
      undef @template_repl;
    }
    elsif(  /^\s*#\s*FPP_TEMPLATE\s+<(\w+)>\s*=\s*(.*)$/  ) {
      $template_name[$template_count] = $1;
      $template_subs[$template_count] = $2;
      $template_count++;
    }
    elsif(  /^\s*#\s*FPP_TEMPLATE_END/  ) {
      close(*FTPLATE);
      # switch stdout back
      *STDOUT = *STDOUT_PREVIOUS;
      @template_junk   = split( /;\s*/, $template_subs[0] );
      $template_copies = @template_junk;
      for( $t=0; $t<$template_copies; $t++ ) {
        for( $n=0; $n<$template_count; $n++ ) {
          @template_junk     = split( /;\s*/, $template_subs[$n] );
          $template_repl[$n] = $template_junk[$t];
        }
        open( FTPLATE, "<$ftplate_file" );
        while( <FTPLATE> ) {
          for( $n=0; $n<$template_count; $n++ ) {
            s/<$template_name[$n]>/$template_repl[$n]/g;
          }
          print;
        }
        close(FTPLATE);
      }
      unlink $ftplate_file;
    }

    elsif(   /^\s*#\s*if/       ||  /^\s*#\s*elif/  
	  || /^\s*#\s*include/  ||  /^\s*#\s*line/
          || /^\s*#\s*error/    ||  /^\s*#\s*cpu/   
	  || /^\s*#\s*system/   ||  /^\s*#\s*assert/
          || /^\s*#\s*unassert/ ||  /^\s*#\s*ident/ 
	  || /^\s*#\s*pragma/   ||  /^\s*#\s*elseif/ ) {
      # error stop if any of these show up.
      # any other lines starting with # are printed (if $newok)
      print STDERR "Error: unsupported preproc stmt: $_\n";
      exit 1;
    }
    elsif( $newok ) { 

      if( $macro_substitute ) {
        # macro substitution
        foreach $k (keys(%subs)) {
          $val = $subs{$k};
          s/(^|\W)$k(\W|$)/\1${val}\2/;
        }
      }

      print;
    }
    else {
      print "\n"   if $blanks;
    }
  }
}

exit;
