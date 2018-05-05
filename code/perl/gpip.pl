#!/usr/bin/perl -w
# Copyright 2002 Ed Avis.  See the file COPYING.
use strict; use warnings;
use Carp qw( carp croak );
use File::Temp qw( tempfile tempdir );
use Getopt::Long qw(:config gnu_getopt no_ignore_case);
use IO::Handle;

my $VERSION = '1.2';
# Whether to remove the temporary directory after running.  Normally
# on, but turning it off may be useful for debugging.
my $CLEANUP = 1;

###############################################################################

sub show_usage() {
    print STDERR <<~"END";
        usage: $0 [-I] [-O] [-i|-o|-b]... PROG ARGS...
        where some ARGS are - or -.foo, if lowercase switches have been used.
        -i replaces the placeholder with a file containing standard input,
        -o replaces it with a file whose contents are output after running,
        -b does both.
        Use the placeholder -.foo to make a temporary file ending '.foo'.

        -I means buffer standard input to make it seekable, but does not change
        the arguments passed to PROG.  -O does the same for standard output.

        Report bugs to <ed\@membled.com>.
        END
}

sub find_placeholder( $ ) {
    use vars '@l';
    local *l = shift;
    my $i = 0;
    foreach (@l) {
        return $i if /^-(\..+|)$/;
        ++$i;
    }
    return undef;
}

# tmpnam()
#
# Return a name for a temporary file.  I believe this is secure
# because it creates a 'private' directory and then uses that.  Cannot
# use POSIX's tmpnam() because I want to ensure 8.3 filenames (and
# anyway, this ought to be more secure).
sub tmpnam() {
    my $tempdir;
    my $num;
    croak 'usage: tmpnam()' if @_;
    for ($tempdir) {
        if ( not defined ) {
            $_ = tempdir( 'XXXXXXXX', CLEANUP => $CLEANUP, TMPDIR => 1 );
            croak 'cannot make temporary directory' if not defined;
            croak "failed to make directory $_" if not -d;
        }
    }

    # Pick an unused filename in this directory.  Since we created the
    # dir ourselves we don't need to look at what it contains.
    #
    $num = 0 if not defined $num;
    return "$tempdir/" . $num++;
}

###############################################################################
# Main

if ( @ARGV < 1 ) {
    show_usage;
    exit 1;
}

# Split the arguments into flags, and the rest.
my $flags = '';
my @rest  = ();

# Get the flags into a big lump.
while ( my $arg = shift @ARGV ) {
    if ( index( '--help', $arg ) == 0 ) {
        show_usage();
        exit(0);
    }
    elsif ( index( '--version', $arg ) == 0 ) {
        print STDERR <<~"END";
            gpip $VERSION
            Written by Ed Avis.

            Copyright (C) 2002 Ed Avis.
            This is free software; see the source for copying conditions.  There is NO
            warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
            END
        exit(0);
    }
    elsif ( $arg =~ /^--/ ) {
        print STDERR "unknown option $arg\n";
        show_usage();
        exit(1);
    }
    elsif ( $arg =~ /^-(.*)/ ) {
        local $_ = $1;

        if (tr/iobIO//c) {
            croak "bad switch $_, expected one of: -i -o -b -I -O";
        }
        elsif ( $_ eq '' ) {
            croak "argument '-' must come after name of program to run";
        }
        else {
            $flags .= $_;
        }
    }
    else {
        # Not a flag, push it back and finish.
        unshift @ARGV, $arg;
        last;
    }
}

@rest = @ARGV;
croak "no program specified, usage: $0 [-i|-o]... PROG ARGS..."
    if @rest == 0;


###############################################################################

my ( $prog, @args ) = @rest;
my @infiles;
my @outfiles;
my %is_outfile;
my ( $stdin_file, $stdout_file );

###############################################################################

# Go through all the flags, substituting filenames for '-'
# arguments.
#
my $flag;
foreach $flag ( split( //, $flags ) ) {
    my $found = 0;
    if ( $flag eq 'i' or $flag eq 'o' or $flag eq 'b' ) {
        my $is_in  = ( $flag eq 'i' or $flag eq 'b' );
        my $is_out = ( $flag eq 'o' or $flag eq 'b' );
        my $n      = find_placeholder( \@args );
        croak "no '-' argument found for -i\n" if not defined $n;
        for ( $args[$n] ) {
            /^-(.*)/ or croak;
            $_ = tmpnam() . $1;
            push @infiles,  $_ if $is_in;
            push @outfiles, $_ if $is_out;
            $is_outfile{$_} = $is_out;
        }
    }
    elsif ( $flag eq 'I' ) {
        for ($stdin_file) {
            warn '-I seen twice' if defined;
            $_ = tmpnam();
            push @infiles, $_;
        }
    }
    elsif ( $flag eq 'O' ) {
        for ($stdout_file) {
            warn '-O seen twice' if defined;
            $_ = tmpnam();
            $is_outfile{$_} = 1;
        }
    }
    else {
        croak;    # shouldn't happen, we filtered earlier
    }
}

# The file buffering the program's stdout is printed first.
unshift @outfiles, $stdout_file if defined $stdout_file;

# Get stdin if necessary.  Each input file gets the same data.
if (@infiles) {
    my @handles = ();
    foreach (@infiles) {
        my $fh = new IO::Handle;
        open( $fh, ">$_" ) or croak "can't write to $_: $!";
        push @handles, $fh;
    }

    while (<STDIN>) {
        my $handle;
        foreach $handle (@handles) {
            print $handle $_;
        }
    }

    foreach (@handles) {
        close $_;
    }
}

# Run the program, with stdin and stdout redirected if appropriate.
local *OLDOUT;
if ( defined $stdin_file ) {

    # Won't need to restore stdin afterwards.
    open( STDIN, $stdin_file ) or croak "cannot reopen $stdin_file: $!";
}
if ( defined $stdout_file ) {
    open( OLDOUT, '>&STDOUT' ) or croak "cannot dup stdout: $!";
    open( STDOUT, ">$stdout_file" )
        or croak "cannot reopen $stdout_file for writing: $!";
}
system( $prog, @args );
if ( defined $stdout_file ) {
    open( STDOUT, '>&OLDOUT' )
        or croak "cannot dup stdout back again: $!";
}

if ($CLEANUP) {
    # Remove input files.
    foreach (@infiles) {
        next if $is_outfile{$_};
        ( not -e $_ ) or unlink or croak "cannot unlink $_: $!";
    }
}

# Print output if necessary, and remove files.
my $outfile;
foreach $outfile (@outfiles) {
    unless ( open( OUTFILE, $outfile ) ) {
        if (    $! =~ /^No such file or directory/
            and $outfile !~ m!\.[^/]*$! )
        {
            # Sometimes DOSish programs add an extension to the output
            # filename without being asked.  Sniff around and see if
            # we can find any evidence of this.
            #
            if ( -e $outfile ) {
                croak "open() said $outfile doesn't exist, but it does";
            }
            my @poss = <$outfile.*>;
            if ( @poss == 0 ) {

                # Nope, nothing.
                croak "$prog didn't create $outfile or any $outfile.*\n";
            }
            elsif ( @poss == 1 ) {

                # It looks like the program has indeed created an
                # output file with a silly name.
                #
                my $o = $poss[0];
                $o =~ /^$outfile(\..*)$/ or croak;
                my $ext = $1;
                warn <<~"END";
                    $prog has created the file '$o' instead of '$outfile'.  Perhaps you
                    should have given the output placeholder as '-$1' instead of '-'?
                    END
                open( OUTFILE, $o ) or croak "cannot open $o: $!";
                $outfile = $o;
            }
            else {
                my $s = join( ', ', @poss ) ;
                croak <<~"EOF";
                    '$prog' did not create '$outfile', but it did create: '$s'.
                    I can't handle this sort of thing, giving up.
                    EOF
            }
        }
        else {
            croak "cannot open $outfile: $!";
        }
    }

    while (<OUTFILE>) {
        print;
    }
    close OUTFILE;
    if ($CLEANUP) {
        unlink $outfile or croak "cannot unlink $outfile: $!";
    }
}

