#!/usr/bin/env perl
use strict; use warnings; use v5.26;
use feature qw( signatures );
no warnings qw( experimental::signatures );

use Getopt::Long qw(:config gnu_getopt no_ignore_case);
use Data::Dumper qw( Dumper );
use List::MoreUtils qw( uniq );
use Carp;
use Carp::Assert;

###############################################################################
# Globals and Prototypes

our $numcards;
our $fname = 'shuffledDeck.txt';
#our $fname = 'bad_deck4';

###############################################################################
# Main Functions

#==============================================================================
# Check file validity

sub validate_deck($deck) {
    say scalar(@{$deck});
    assert scalar(@{$deck}) == 52, 'Deck is the wrong size';
    assert scalar(@{$deck}) == scalar(uniq(@{$deck})),
           'Deck contains non unique cards.';

    my $buf = join('', @{$deck});
    assert length($buf) == 104,
           'All cards must be exactly two characters long';
    
    assert $buf =~ /(?:[AKQJ02-9][CDHS]){52}/, 'Incomplete deck.';
}

#==============================================================================

###############################################################################
# Utility Functions

sub get_data($fname) {
    open( 'fh', '<', $fname ) or croak $!;
    my @data;

    while (<fh>) {
        next if /^\s*$/;
        chomp;
        push @data, uc;
    }
    close('fh') or croak $!;

    return @data;
}

sub show_usage {
    print <<"EOF";
Usage: $0 [options] filename
EOF
    exit;
}

my ( $n1, $n2, $n3, $ncards, $help );
GetOptions('numcards|n=i' => \$ncards,
            '1'            => \$n1,
            '2'            => \$n2,
            '3'            => \$n3,
            'help|h'       => \$help
    ) or croak "Invalid argument.";

if ( $n1 or $n2 or $n3 ) {
    if    ( $n1 ) { $numcards = 1; }
    elsif ( $n2 ) { $numcards = 2; }
    elsif ( $n3 ) { $numcards = 3; }
}
elsif ( defined($ncards) ) {
    if ( $ncards =~ /^\d+$/ and 0 < $ncards and $ncards <= 3 ) {
        $numcards = $ncards;
    } else {
        croak 'Invalid value for numcards - must be an integer between '.
                ' 1 and 3 (inclusive).';
    }
}
elsif ( $help ) {
    show_usage;
}

###############################################################################
# Main

my @data = get_data $fname;

validate_deck \@data;

#print Dumper(\@data);


