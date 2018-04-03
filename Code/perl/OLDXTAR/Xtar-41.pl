#!/usr/bin/env perl
use warnings; use strict; use v5.26;
use feature 'signatures';
no warnings 'experimental::signatures';
use Cwd qw( getcwd realpath );
use Carp;
use File::Basename;
use File::Which;
use Getopt::Long qw(:config gnu_getopt no_ignore_case);

use lib "$ENV{HOME}/random/Code/perl/xtar";
use xtar;
use xtar::Colors qw( sayC fsayC esayC );

###############################################################################
# Main

my ( %options, $TAR );

my $default_tar = 'bsdtar';

GetOptions(
    'h|help'    => \$options{help},
    'V|version' => \$options{version},
    'v|verbose' => \$options{verbose},
    'c|combine' => \$options{combine},
    'b|bsdtar'  => \$options{bsdtar},
    'g|gtar'    => \$options{gtar},
    'T|tar=s'   => \$options{tar},
    'o|out=s'   => \$options{odir}
) or show_usage(1);

if ( $options{help} )    { show_usage(0) }
if ( $options{version} ) { say "xtar version 4.0.0 (perl)" and exit 0 }

unless (@ARGV) {
    print STDERR "Error: No input files.\n\n";
    show_usage(1);
}

if    ( defined $options{tar} ) { $TAR = find_tar( $options{tar} ) }
elsif ( $options{bsdtar} )      { $TAR = find_tar('bsdtar') }
elsif ( $options{gtar} )        { $TAR = find_tar('gtar') }
else                            { $TAR = find_tar( $default_tar ) }

if ( $options{combine} ) { die "Combine is not implemented yet." }
if ( $options{verbose} ) { warn "Verbose doesn't do anything yet." }


sub find_tar($binary) {
    if   ( which($binary) ) { return $binary }
    else                    { return 'tar' }
}


###############################################################################


my $xtar = xtar->new(
    Options => {
        TAR     => $TAR,
        odir    => $options{odir},
        verbose => $options{verbose},
        combine => $options{combine}
    },
    CWD         => realpath(getcwd()),
    NumArchives => scalar(@ARGV)
);
undef %options;

$xtar->init_outpath();

my $counter = 1;

while (@ARGV) {
    my $file = shift;

    print "\n\n" if ( $counter++ > 1 );
    sayC( 'yellow', "----- Processing file '$file' -----" );

    $xtar->init_archive($file);
    $xtar->extract();
}


#-----------------------------------------------------------------------------
###############################################################################
#-----------------------------------------------------------------------------


sub show_usage($status) {
    my $THIS = basename $0;
    if ( $status == 0 ) {
        print "Usage: ${THIS} [options] archive(s)...\n\n";
        print << 'EOF';
Extract an archive safely to a unique directory, ensuring no irritating
single sub-directories or lots of loose files are created. See the manual for
more detailed information.

OPTIONS
 -h --help      Show this usage information.
 -V --version   Show version.
 -v --verbose   Verbose mode. Display progress information if possible.
 -o --out [DIR] Explicitly specify output directory. If it already exists,
                time_t will be appended to it. When used with multiple
                archives it will function as a top directory with each archive
                extracted to sub-directories, unless -c is supplied, whereupon
                all archives are combined into it.
 -c --combine   Combine multiple archives. When -o is not supplied, a directory
                name is generated from the name of the first supplied archive.
                *** NOT IMPLEMENTED ***
 -T --tar [ARG] Explicity specify the tar binary to use.
 -b --bsdtar    Use bsdtar over 'tar' if it exists, otherwise fall back to tar.
 -g --gtar      Use gtar if it exists, otherwise fall back to tar.
EOF
    }
    else { say STDERR "Usage: ${THIS} [options] archive(s)..." }

    exit $status;
}
