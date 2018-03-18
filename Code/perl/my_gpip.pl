#!/usr/bin/perl
use strict; use warnings; use v5.26;
use feature qw( signatures );
no warnings qw( experimental::signatures );
use Carp qw( carp croak );
use Data::Dumper qw( Dumper );
use File::Temp qw( tempfile cleanup );
use File::Which;

###############################################################################

sub get_cmd($program) {
    defined($program) or croak 'No program identified in commandline!';
    my $cmd = which($program) or croak "Command '$program' not found.";
    return $cmd;
}

sub get_tmp($type) {
    if ( defined($type) ) {
        return tempfile SUFFIX => ".$type", TMPDIR => 1, CLEANUP => 1 ;
    } else {
        return tempfile TMPDIR => 1, CLEANUP => 1;
    }
}

###############################################################################

croak 'No arguments!' if @ARGV == 0;
my ( $type, $program, $position, @args );

my $iter = 0;
while (@ARGV) {
    my $param = shift;

    if ( $iter == 0 ) {
        if ( $param =~ /^-(.*)/ ) {
            $type = $1;
        } else {
            $program = $param;
        }
    }
    elsif ( $iter == 1 and not defined($program) ) {
        $program = $param;
    }
    elsif ( not defined($type) and $param =~ /^-\.(.*)/ ) {
        $type     = $1;
        $position = $iter - 1;
        push @args, '';
    }
    elsif ( not defined($position) and $param eq '-' ) {
        $position = $iter - 2;
        push @args, '';
    }
    else {
        push @args, $param;
    }
    ++$iter;
}

###############################################################################
# Now some processing

$program = get_cmd $program;
my ( $fh, $tmpname ) = get_tmp $type;

if ( defined($position) ) {
    $args[$position] = $tmpname;
} else {
    push @args, $tmpname;
}

# Dump stdin to the file.
while (<>) {
    print $fh $_;
}

# Run the command, wait a bit for programs like firefox that automatically
# fork and separate themselves to open the file, then delete the file and exit.
system( $program, @args );
sleep 1;
unlink $tmpname or carp "Couldn't delete temporary file '$tmpname'.";
