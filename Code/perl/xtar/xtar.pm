package xtar;
use warnings; use strict; use v5.26;
use Mouse;
use feature 'signatures';
no warnings 'experimental::signatures';

use boolean;
use Carp;
use File::Basename;
#use Data::Dumper;
use Cwd qw( realpath getcwd );
use File::Temp qw( tempdir cleanup );
use File::Copy qw( mv );
use File::Which;

use lib "$ENV{HOME}/random/Code/perl/xtar";
use xtar::File;
use xtar::OutPath;
use xtar::Colors qw( sayC fsayC esayC );

###############################################################################

has 'CWD'         => ( is => 'ro', isa => 'Str' );
has 'Options'     => ( is => 'ro', isa => 'HashRef' );
has 'NumArchives' => ( is => 'ro', isa => 'Int' );
has 'counter'     => ( is => 'rw', isa => 'Int' );
has 'archive'     => ( is => 'rw', isa => 'Object' );
has 'out'         => ( is => 'rw', isa => 'Object' );
has 'tmpdir'      => ( is => 'rw', isa => 'Str' );

my $cmd_color = 'Bmagenta';

###############################################################################


sub init_outpath($self)
{
    $self->out(
        xtar::OutPath->new(
            CWD         => $self->CWD,
            Options     => $self->Options,
            NumArchives => $self->NumArchives
        )
    );
}


sub init_archive( $self, $filename )
{
    $self->archive( xtar::File->new( $filename ) );
    $self->archive->analysis();
    $self->out->init( $self->archive );
}


sub extract($self)
{
    my $lonefile = true;

    while ( $lonefile ) {
        unless ( $self->__extract() ) { return false }
        
        $lonefile = $self->out->analyze_output( $self->tmpdir );

        if ( $lonefile ) {
            esayC( 'yellow', 'The output contains only a single file.',
                   "It could be a sub-archive. Attempting to extract.\n" );
            
            $self->init_archive( $lonefile );
        }
    }

    mv( $self->out->bottom, $self->out->odir ) or croak "$!";

    my $CWD    = $self->CWD;
    my $reldir = $self->out->odir =~ s|${CWD}/(.*)|$1|r;
    my $odir   = $self->out->odir;
    sayC( 'yellow', "Extracted to $reldir" );
}


###############################################################################


sub __extract($self)
{
    my $success = false;
    my $tmpdir;

    foreach my $try ( ('likely', 'mime', 'ext') ) {
        my $cmd    = eval qq( \$self->archive->${try}_cmd );
        my $is_tar = eval qq( \$self->archive->${try}_tar );

        next unless ( $cmd->{CMD} );

        $tmpdir = get_tempdir( $self->CWD, $self->archive->fullpath );
        chdir $tmpdir;

        if ( do_extraction( $self->archive->fullpath, $cmd, $is_tar, $self->Options ) ) {
            esayC( 'green', 'Operation appears successful.' );
            $success = true;
            last;
        }
        else { esayC( 'red', "Operation failed.\n" ) }

        chdir $self->CWD;
    }

    unless ( $success ) {
        esayC( 'Bred', 'All identified programs have failed.' );

        $tmpdir = get_tempdir( $self->CWD, $self->archive->fullpath );
        chdir $tmpdir;

        $success = force_extract( $self->archive->fullpath,
                                  $self->Options->{'TAR'} );
    }

    $self->tmpdir( realpath( $tmpdir ) );
    chdir $self->CWD;
    return $success;
}


###############################################################################


sub do_extraction( $archive, $cmd, $is_tar, $options )
{
    if ( $cmd->{CMD} eq 'FAIL' ) {
        return false
    }
    elsif ( $is_tar and $cmd->{tflags} eq 'NOTAR' ) {
        return extract_else( $cmd->{CMD}, $cmd->{eflags}, $cmd->{stdout},
                             $archive, $options );
    }
    else {
        if ($is_tar) {
            return extract_tar( $cmd->{CMD}, $cmd->{tflags},
                                $archive, $options );
        }
        else {
            return extract_else( $cmd->{CMD}, $cmd->{eflags}, $cmd->{stdout},
                                 $archive, $options );
        }
    }
}


sub extract_tar( $CMD, $flags, $file, $options )
{
    my $ret = true;
    my $shortname = basename( $file );

    my $command = substitute_cmd( $CMD, $flags, $file, $options );
    my $short_command = substitute_cmd( $CMD, $flags, $shortname, $options );

    if ( $flags eq 'SPECIAL' ) { croak }
    else {
        sayC( $cmd_color, qq($short_command | $options->{TAR} -xf -) );

        my ( $CmdPipe, $TarPipe );
        open $CmdPipe, '-|', qq($command);
        open $TarPipe, '|-', qq($options->{TAR} -xf -);

        while ( <$CmdPipe> ) {
            print $TarPipe $_;
        }

        # If either command failed we won't be able to close its pipe.
        close $CmdPipe or $ret = false;
        close $TarPipe or $ret = false;
    }

    return $ret;
}


sub extract_else( $CMD, $flags, $stdout, $file, $options )
{
    my $shortname = basename( $file );
    my $CWD = getcwd();

    my $command = substitute_cmd( $CMD, $flags, $file, $options );
    my $short_command = substitute_cmd( $CMD, $flags, $shortname, $options );

    if ( $flags eq 'SPECIAL' ) { croak }
    else {
        if ($stdout) {
            sayC( $cmd_color, qq($short_command > "$CWD/") );
            system qq($command > "$CWD/$shortname");
        }
        else {
            sayC( $cmd_color, qq($short_command) );
            system qq($command);
        }
    }

    return boolean( $? == 0 );
}


###############################################################################


sub substitute_cmd ( $CMD, $flags, $file, $options )
{
    my $TAR = $options->{TAR};
    $CMD =~ s/TAR/$TAR/;

    if ( $flags =~ /--/ ) {
        $flags =~ s/--/'$file'/;
    }
    else {
        $flags .= " '$file'";
    }

    return "$CMD $flags";
}


sub get_tempdir( $CWD, $archive )
{
    my @archive_info = stat $archive;
    my @dest_info    = stat '/tmp';
    my $C = 1;

    if ( $archive_info[0] == $dest_info[0] ) {
        return tempdir( CLEANUP => $C );
    }

    @dest_info = stat $CWD;
    if ( $archive_info[0] == $dest_info[0] and -w $CWD ) {
        return tempdir( CLEANUP => $C, DIR => $CWD );
    }

    @dest_info = stat( $ENV{'HOME'} );
    if ( $archive_info[0] == $dest_info[0] ) {
        return tempdir( CLEANUP => $C, DIR => $ENV{'HOME'} );
    }

    return tempdir( CLEANUP => $C );
}


###############################################################################


sub force_extract( $archive, $TAR )
{
    esayC( 'Bred', "Attempting to force extract.\n" );
    my $index = 1;
    my $color = 'Byellow';

    while (true) {
        if ( $index == 1 ) {
            esayC( $color, "Trying tar" );
            system qq($TAR -xf "$archive");
            last if ( $? == 0 );
        }
        elsif ( $index == 2 and which('patool') ) {
            esayC( $color, "\nTrying patool" );
            system qq(patool extract "$archive");
            last if ( $? == 0 );
        }
        elsif ( $index == 3 and which('atool') ) {
            esayC( $color, "\nTrying patool" );
            system qq(atool -x "$archive");
            last if ( $? == 0 );
        }
        elsif ( $index == 4 and which('7z') ) {
            esayC( $color, "\nTrying 7zip" );
            system qq(7z x "$archive");
            last if ( $? == 0 );
        }
        elsif ( $index == 5 and which('zpaq') ) {
            esayC( $color, "\nTrying zpaq" );
            system qq(zpaq x "$archive" -to tmp);
            last if ( $? == 0 );
        }
        elsif ( $index == 6 ) {
            esayC( 'Bred', "\n\nTotal failure. Giving up" );
            return false;
        }

        ++$index;
    }

    esayC( 'Bgreen', 'Success!' );
    return true;
}


###############################################################################

no Mouse;
__PACKAGE__->meta->make_immutable;
