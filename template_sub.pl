#!/usr/bin/perl

use strict;
use warnings;

my $string;

{
  local $/ = undef;
  open FILE, $ARGV[0] or die "Couldn't open file: $!";
  binmode FILE;
  $string = <FILE>;
  close FILE;
}

sub my_boye {
  local $/ = undef;
  open FILE, "src/template/$_[0]" or die "Couldn't open file: $!";
  binmode FILE;
  my $string = <FILE>;
  close FILE;

  return $string;
}

$string =~ s/\{\{([a-z0-9A-Z.]*)\}\}/my_boye($1)/ge;

print $string;

