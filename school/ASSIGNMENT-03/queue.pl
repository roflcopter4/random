#!/usr/bin/env perl
use strict; use warnings; use v5.26;
use feature qw( signatures );
use Carp;

###############################################################################
# Packages

# Queue attempt
{

    package myQueue;
    use Moose;
    no warnings qw( experimental::signatures );

    has 'items' => (
        is  => 'rw',
        isa => 'ArrayRef',
    );
    has 'count' => (
        is  => 'rw',
        isa => 'Int'
    );
    has 'capacity' => (
        is  => 'ro',
        isa => 'Int'
    );
    has 'head' => (
        is  => 'rw',
        isa => 'Int'
    );
    has 'tail' => (
        is  => 'rw',
        isa => 'Int'
    );

    around BUILDARGS => sub {
        my $orig  = shift;
        my $class = shift;

        if ( @_ == 1 && !ref( $_[0] ) ) {
            my $capacity = $_[0];
            confess "Invalid capacity." unless $capacity > 2;

            my @items;
            for ( my $i = 0; $i < $capacity; ++$i ) {
                push @items, undef;
            }
            return $class->$orig(
                items    => \@items,
                capacity => $capacity,
                count    => 0,
                head     => 0,
                tail     => 0
            );

        }
        else {
            return $class->$orig(@_);
        }

        super();
    };

    # ------------------------------------------------------------------------

    sub enqueue ( $self, $item ) {
        ( $self->count < $self->capacity ) or croak('Queue is full!');
        $self->items->[ $self->tail ] = $item;
        $self->tail( ( $self->tail + 1 ) % $self->capacity );
        $self->count( $self->count + 1 );
    }

    sub dequeue($self) {
        ( $self->count > 0 ) or croak('Queue is empty!');
        my $item = $self->items->[ $self->head ];
        $self->items->[ $self->head ] = undef;
        $self->head( ( $self->head + 1 ) % $self->capacity );
        $self->count( $self->count - 1 );

        return $item;
    }

    sub peek($self) {
        if ( $self->count > 0 ) {
            return $self->items->[ $self->head ];
        }
        else {
            return undef;
        }
    }

    sub get_size($self) {
        return $self->count;
    }

    sub is_full($self) {
        return ( $self->count == ( $self->capacity - 1 ) );
    }

    sub is_empty($self) {
        return ( $self->count == 0 );
    }

    sub repr($self) {
        my $buf = sprintf( 'Head: %d, Tail: %d, Capacity: %d/%d',
                    $self->head, $self->tail, $self->count, $self->capacity );
        my @tmp;
        foreach ( @{ $self->items } ) {
            push @tmp, $_ if defined;
        }
        return "${buf}\n[" . join( ', ', @tmp ) . "]\n";
    }

}

#==============================================================================
# Small extension of myQueue.
{

    package myHand;
    use Moose;
    extends 'myQueue';
    no warnings qw( experimental::signatures );

    sub get_card($self) {
        if ( $self->is_empty() ) {
            return undef;
        }
        else {
            return $self->dequeue();
        }
    }

    sub add_cards {
        my $self = shift;
        while (shift) {
            $self->enqueue($_);
        }
    }

    __PACKAGE__->meta->make_immutable;
}

###############################################################################
# Init

no warnings qw( experimental::signatures );

use Getopt::Long qw(:config gnu_getopt no_ignore_case);
use Data::Dumper qw( Dumper );
use List::Util qw( shuffle );
use List::MoreUtils qw( uniq );

use myHand;
our $numcards;
our $fname;

###############################################################################
# Main Functions

sub validate_deck($deck) {
    ( @{$deck} == 52 ) or die 'ERROR: "Deck is the wrong size."';
    ( uniq( @{$deck} ) == 52 )
        or die 'ERROR: "Deck contains non unique cards."';

    my $buf = join( '', @{$deck} );
    ( length($buf) == 52 * 2 )
        or die 'ERROR: "All cards must be exactly 2 characters long."';
    ( $buf =~ /(?:[AKQJ02-9][CDHS]){52}/ )
        or die 'ERROR: "Deck is incomplete."';
}

sub distribute_cards($deck) {
    my @hands    = ( myHand->new(52), myHand->new(52) );
    my @shuffled = shuffle @{$deck};
    my $plr      = 0;

    foreach my $card (@shuffled) {
        $hands[$plr]->enqueue($card);
        $plr = ( $plr + 1 ) % 2;
    }
    return @hands;
}

#===============================================================================
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
    print <<~"EOF";
        Usage: $0 [options] <file>\n
        File should be a valid filename containing formatted deck information.
        -h --help           Show this help and exit.
        -n --numcards=[N]   Specify number of cards placed on the table in war.
        -1 -2 -3            Aliases for -n1 -n2 -n3.
        \nValues not specified on the command line can be given interactively.
        EOF
    exit;
}

#==============================================================================
# Commandline argument handling.
{
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
}

###############################################################################
# Main

my @deck = get_data( $fname );
validate_deck( \@deck );
my @hands = distribute_cards( \@deck );

#print Dumper(@hands);

map {say $_->repr()} @hands;
