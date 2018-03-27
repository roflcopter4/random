package xtar::OutPath;
use warnings; use strict; use v5.26;
use Moose;
use feature 'signatures';
no warnings 'experimental::signatures';

use boolean;
use Carp;
#use Data::Dumper;
use File::Basename;
use Cwd qw( realpath );

###############################################################################

has 'CWD'         => ( is => 'ro', isa => 'Str' );
has 'Options'     => ( is => 'ro', isa => 'HashRef' );
has 'NumArchives' => ( is => 'ro', isa => 'Int' );

has 'tmpdir'      => ( is => 'rw', isa => 'Str' );
has 'bottom'      => ( is => 'rw', isa => 'Str' );
has 'top_dir'     => ( is => 'rw', isa => 'Str' );
has 'odir'        => ( is => 'rw', isa => 'Str' );

has 'archive'     => ( is => 'rw', isa => 'Object' );
has 'notfirst'    => ( is => 'rw', isa => 'Bool' );

###############################################################################


sub init( $self, $archive )
{
    $self->archive( $archive );

    if ( $self->Options->{'odir'}) {
        unless ( $self->notfirst  ) {
            my $top = realpath( $self->Options->{'odir'} );
            if ( -e $top ) {
                $top = handle_conflict( dirname( $top ), basename( $top ) );
            }

            my $tmp = $top;
            while ( true ) {
                if    ( not -e $tmp ) { $tmp = dirname( $tmp ) }
                elsif ( not -w $tmp ) {
                    croak "Given output directory is not writable. Aborting.";
                }
                else { last }
            }

            $self->top_dir( $top );
        }
    }
    else {
        unless ( -w $self->CWD ) {
            croak "Current directory is not writable. Aborting.";
        }
        $self->top_dir( $self->CWD );
    }

    $self->notfirst( true );
}


###############################################################################


sub analyze_output( $self, $tmpdir )
{
    my ( $bottom, $lonefile ) = descend( $tmpdir );
    chdir $self->CWD;
    $self->bottom( $bottom );
    $self->tmpdir( $tmpdir );

    $self->get_odir();

    return $lonefile;
}


sub get_odir($self)
{
    if ( $self->Options->{'odir'} and $self->NumArchives == 1 ) {
        $self->odir( $self->top_dir );
    }
    else {
        $self->odir( $self->__get_odir() );
    }
}


sub __get_odir($self)
{
    my $oname;

    if ( $self->tmpdir eq $self->bottom ) {
        $oname = $self->archive->bname;
    }
    else {
        $oname = basename( $self->bottom );
    }

    my $odir = sprintf( "%s/%s", $self->top_dir, $oname );
    if ( -e $odir ) {
        $odir = handle_conflict( $self->top_dir, $oname );
    }

    return $odir;
}


###############################################################################


sub handle_conflict( $path, $name )
{
    my $TimeStamp = time;
    my $new_name  = "${path}/${name}-${TimeStamp}";

    my $i = 1;
    while ( -e $new_name ) {
        $new_name = "${path}/${name}-${TimeStamp}-${i}";
        ++$i;
    }

    return $new_name;
}


sub glob_all
{
    my @files = glob('* .*');
    my @filter;

    foreach my $cur (@files) {
        next if ( $cur eq '.' or $cur eq '..' );
        push @filter, $cur;
    }

    return @filter;
}


sub descend($dir)
{
    chdir $dir;
    my @files = glob_all();
    my $bottom;
    my $lonefile = '';

    if ( @files == 1) {
        if ( -d $files[0] ) {
            my $lonedir = realpath( $files[0] );
            ( $bottom, $lonefile ) = descend( $lonedir );
        }
        else {
            $bottom   = $dir;
            $lonefile = realpath( $files[0] );
        }
    }
    elsif ( @files == 0 ) {
        croak "No files in output directory!";
    }
    else {
        $bottom = $dir;
    }

    return ( $bottom, $lonefile );
}


###############################################################################

no Moose;
__PACKAGE__->meta->make_immutable;
