package xtar;
use warnings; use strict; use v5.26;
use Mouse;
use feature 'signatures';
no warnings 'experimental::signatures';

use constant true  => 1;
use constant false => 0;
use Carp;
use File::Which;
use File::Basename;
use Data::Dumper;
use Cwd 'getcwd';
use File::Copy qw( mv cp );
use File::Path qw( make_path );
use File::Temp qw( tempdir cleanup );
use File::Copy::Recursive 'dircopy';
use File::Spec::Functions 'rel2abs';

use lib "$ENV{HOME}/random/Code/perl/xtar";
use xtar::File;
use xtar::OutPath;
use xtar::Colors qw( sayC fsayC esayC );

###############################################################################

has 'CWD'         => ( is => 'ro', isa => 'Str' );
has 'Options'     => ( is => 'ro', isa => 'HashRef' );
has 'NumArchives' => ( is => 'ro', isa => 'Int' );
has 'counter'     => ( is => 'rw', isa => 'Int' );
has 'tmpdir'      => ( is => 'rw', isa => 'Str' );

has 'out'         => ( is => 'rw', isa => 'Object' );
has 'file'        => ( is => 'rw', isa => 'Object' );

my $cmd_color = 'Bcyan';
our $DEBUG;

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
    $DEBUG = $self->Options->{'Debug'};
}


sub init_archive( $self, $filename )
{
    $self->file( xtar::File->new( $filename, $self->Options ) );
    $self->file->analysis();
    $self->out->init( $self->file );
}


###############################################################################


sub extract($self)
{
    my $lonefile = true;

    while ($lonefile) {
        unless ( $self->try_extractions() ) { return false }
        
        $lonefile = $self->out->analyze_output($self->tmpdir);

        if ($lonefile) {
            esayC( 'bRED', 'The output contains only a single file.',
                   "It could be a sub-archive. Attempting to extract.\n" );
            
            $self->init_archive($lonefile);
        }
    }

    # What a mess of a command follows here.
    safe_make_path( $self->out->top_dir );

    mv( $self->out->bottom, $self->out->odir )
      or ( $DEBUG && say STDERR "Resorting to dircopy" or true )
         && dircopy( $self->out->bottom, $self->out->odir )
      or confess "Dircopy failed. Aborting. - $!";

    my $CWD    = $self->CWD;
    my $reldir = $self->out->odir =~ s|${CWD}/(.*)|$1|r;
    my $odir   = $self->out->odir;

    sayC( 'bGREEN', "Extracted to $reldir" );
}


sub _get_tempdir($self)
{
    my @odir_info = stat $self->out->top_exist;
    my $C = 1;
    my $dir;
    
    if ( $odir_info[0] == [stat '/tmp']->[0] ) {
        $dir = tempdir( CLEANUP => $C );
    }
    elsif ( -w $self->out->top_exist ) {
        $dir = tempdir( CLEANUP => $C, DIR => $self->out->top_exist );
    }
    elsif ( $odir_info[0] == [stat $self->CWD]->[0] and -w $self->CWD ) {
        $dir = tempdir( CLEANUP => $C, DIR => $self->CWD );
    }
    elsif ( $odir_info[0] == [$ENV{HOME}]->[0] ) {
        $dir = tempdir( CLEANUP => $C, DIR => $ENV{HOME} );
    }
    else {  # If all else fails...
        $dir = tempdir( CLEANUP => $C );
    }

    say STDERR "Tmpdir is " . rel2abs($dir) if $DEBUG;
    return $dir;
}


###############################################################################


sub try_extractions($self)
{
    my $success = false;
    my $tmpdir;

    foreach my $try ( ('likely', 'mime', 'ext') ) {
        my $cmd    = eval qq( \$self->file->${try}_cmd );
        my $is_tar = eval qq( \$self->file->${try}_tar );

        next unless ( $cmd->{CMD} );

        $tmpdir = $self->_get_tempdir();
        chdir $tmpdir;

        $success = do_extraction( $self->file->fullpath,
                                  $cmd, $is_tar, $self->Options );

        if ( $success ) {
            if ( $self->Options->{'verbose'} ) {
                esayC( 'GREEN', 'Operation appears successful.' );
            }
            last;
        }
        else {
            esayC( 'bRED', "Operation failed.\n" )
        }

        chdir $self->CWD;
    }

    unless ( $success ) {
        esayC( 'bRED', 'All identified programs have failed.' );

        $tmpdir = $self->_get_tempdir();
        chdir $tmpdir;

        $success = force_extract( $self->file->fullpath,
                                  $self->Options->{'TAR'} );
    }

    $self->tmpdir( rel2abs($tmpdir) );
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
    my $shortname = basename($file);

    my $command = substitute_cmd( $CMD, $flags, $file, $options );
    my $short_command = substitute_cmd( $CMD, $flags, $shortname, $options );

    sayC( $cmd_color, qq($short_command | $options->{TAR} -xf -) );

    my ( $CmdPipe, $TarPipe );
    open $CmdPipe, '-|', qq($command);
    open $TarPipe, '|-', qq($options->{TAR} -xf -);

    # We just act like a filter between the commands.
    while (<$CmdPipe>) { print $TarPipe $_ }

    # If either command failed we won't be able to close its pipe.
    close($CmdPipe) or $ret = false;
    close($TarPipe) or $ret = false;

    return $ret;
}


sub extract_else( $CMD, $flags, $stdout, $file, $options )
{
    my $shortname = basename($file);
    my $CWD = getcwd();

    my $command = substitute_cmd( $CMD, $flags, $file, $options );
    my $short_command = substitute_cmd( $CMD, $flags, $shortname, $options );

    if ($stdout) {
        sayC( $cmd_color, qq($short_command > "$CWD/") );
        system qq($command > "$CWD/$shortname");
    }
    else {
        sayC( $cmd_color, qq($short_command) );
        system qq($command);
    }

    return $? == 0;
}


###############################################################################


sub substitute_cmd( $CMD, $flags, $file, $options )
{
    my $TAR = $options->{TAR};
    $CMD =~ s/TAR/$TAR/;

    if ( $flags =~ /--/ ) { $flags =~ s/--/'$file'/ }
    else                  { $flags .= " '$file'" }

    return "$CMD $flags";
}


sub safe_make_path($top_dir)
{
    my $dir = dirname($top_dir);
    make_path($dir) unless ( -e $dir );
}


###############################################################################


sub force_extract( $archive, $TAR )
{
    esayC( 'bRED', "Attempting to force extract.\n" );
    my $index = 1;
    my $color = 'bYELLOW';

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
            esayC( 'bRED', "\n\nTotal failure. Giving up" );
            return false;
        }

        ++$index;
    }

    esayC( 'bGREEN', 'Success!' );
    return true;
}


###############################################################################

no Mouse;
__PACKAGE__->meta->make_immutable;
