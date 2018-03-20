#!/usr/bin/env perl
use strict; use warnings; use v5.26;
use feature qw( signatures );
no warnings qw( experimental::signatures );

use Getopt::Long qw(:config gnu_getopt no_ignore_case);
use Data::Dumper qw( Dumper );
use List::Util qw( shuffle );
use List::MoreUtils qw( uniq );
use Class::Struct;
use Carp;

###############################################################################
# Globals and Prototypes

our $numcards;
our $fname;

struct(
    hand => {
        num  => '$',
        list => '@'
    }
);

###############################################################################
# Main Functions

sub validate_deck($deck) {
    ( @{$deck} == 52 ) or die 'ERROR: "Deck is the wrong size."';
    ( uniq(@{$deck}) == 52 ) or die 'ERROR: "Deck contains non unique cards."';

    my $buf = join( '', @{$deck} );
    ( length($buf) == 52*2 )
        or die 'ERROR: "All cards must be exactly 2 characters long."';
    ( $buf =~ /(?:[AKQJ02-9][CDHS]){52}/ )
        or die 'ERROR: "Deck is incomplete."';
}

sub distribute_cards($deck) {
    my @hands = ( [], [] );
    my @shuffled = shuffle @{$deck};

    my $plr = 0;
    foreach my $card ( @shuffled ) {
        push @{$hands[$plr]}, $card;
        $plr = ($plr + 1) % 2;
    }
    return @hands;
}

###############################################################################
# Utility Functions

sub get_filename {
    print "Enter filename:\n > ";
    $fname = <>;
}

sub get_data($fname) {
    open( 'fh', '<', $fname ) or croak "'$!'";
    my @data;

    while (<fh>) {
        next if /^\s*$/;
        chomp;
        push @data, uc;
    }
    close('fh') or croak "$!";

    return @data;
}

sub show_usage {
    print <<"EOF";
Usage: $0 [options] filename
EOF
    exit;
}

#==============================================================================
# Commandline argument handling.

my ( $n1, $n2, $n3, $ncards, $help );
GetOptions(
    'numcards|n=i' => \$ncards,
    '1'            => \$n1,
    '2'            => \$n2,
    '3'            => \$n3,
    'help|h'       => \$help
) or croak "Invalid argument.";

if ( $n1 or $n2 or $n3 ) {
    if    ($n1) { $numcards = 1; }
    elsif ($n2) { $numcards = 2; }
    elsif ($n3) { $numcards = 3; }
}
elsif ( defined($ncards) ) {
    if ( ($ncards =~ /^\d+$/) and (0 < $ncards) and ($ncards <= 3) ) {
        $numcards = $ncards;
    }
    else {
        warn "ERROR: numcards must be an integer between 1 and 3.\n";
        exit 1;
    }
}
elsif ($help) { show_usage; }

$fname = shift or $fname = get_filename;

###############################################################################
# Main

my @data = get_data $fname;
validate_deck \@data;

my @hands = distribute_cards \@data;
print Dumper(\@hands);

my $f = new hand;
$f->num(52);

print Dumper(\$f);
