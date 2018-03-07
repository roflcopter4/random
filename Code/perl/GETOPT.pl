#!/usr/bin/perl
use warnings; use strict; use 5.26.1;
use Getopt::Long qw(:config gnu_getopt no_ignore_case);

my $length = 24;
my $data = "~/.vimrc";
my $verbose;
GetOptions ("length|l=i" => \$length,    # numeric
            "file|f=s"   => \$data,      # string
            "verbose|v"  => \$verbose)   # flag
    or die("Error in command line arguments.\n");

say "Length = $length";
say "Datafile = $data";
if ($verbose) {
    say "Is verbose.";
} else {
    say "Is NOT verbose.";
}

