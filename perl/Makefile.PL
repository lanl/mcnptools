use strict;
use Config;
use ExtUtils::MakeMaker;
use File::Basename;
use File::Find;
use IO::File;

my @mcnptools_include_dirs = (".", "../libmcnptools/include");
my @mcnptools_source_files = ("mcnptools_wrap.cpp");
find(sub{ if($File::Find::name =~ /\.cpp$/) { 
            push(@mcnptools_source_files, $File::Find::name);
          }
        }, "../libmcnptools/src"
);

my @boost_include_dirs = ("../libmcnptools/boost");
my @boost_compile_defs = ("-D__MCNPTOOLS_BOOST_REGEX__");
my @boost_source_files;
foreach my $bdir ("regex") {
  push(@boost_include_dirs, "../libmcnptools/boost/libs/$bdir/src");
  find(sub{ if($File::Find::name =~ /\.cpp$/) {
              push(@boost_source_files, $File::Find::name);
            }
          }, "../libmcnptools/boost/libs/$bdir/src"
  );
}

my @all_sources;
foreach my $s (@mcnptools_source_files,@boost_source_files) {
  my $b = basename($s);
  symlink($s,$b);
  push(@all_sources,$b);
}

my @all_include_dirs;
foreach my $i (@mcnptools_include_dirs,@boost_include_dirs) {
  push(@all_include_dirs, "-I $i");
}

my $VERSION = "unknown";
if( -e "../VERSION" ) {
  my $fh = new IO::File("../VERSION","r");
  while (my $line=<$fh>) {
    if( $line =~ m/^(.+\..+\..+)$/ ) {
      $VERSION=$1;
    }
  }
}

# system dependent configurations
my @compile_flags = ("-std=c++11");
my @link_flags = ();
if( $Config{osname} =~ m/linux/ ) {
  push(@link_flags,("-lstdc++","-static-libgcc","-static-libstdc++"));
}
elsif( $Config{osname} =~ m/darwin/ ) {
  push(@compile_flags,("-mmacosx-version-min=10.7", "-stdlib=libc++"));
  push(@link_flags,("-mmacosx-version-min=10.7", "-stdlib=libc++"));
}

WriteMakefile(
  NAME => "McnpTools",
  VERSION => $VERSION,
  C => \@all_sources,
  CCFLAGS => "$Config{ccflags} @compile_flags",
  LDDLFLAGS => "$Config{lddlflags} @link_flags",
  LDFLAGS => "$Config{ldflags} @link_flags",
  DEFINE => "@boost_compile_defs",
  INC  => "@all_include_dirs",
  OBJECT => '$(O_FILES)'
);

