package xtar::File;
use warnings; use strict; use v5.26;
use Moose;
use feature 'signatures';
no warnings 'experimental::signatures';

use boolean;
use File::Which;
use File::Basename;
#use File::Unpack;
use Carp qw( carp croak cluck confess );
use Cwd qw( realpath );

##############################################################################

has 'filename'    => ( is => 'ro', isa => 'Str' );
has 'fullpath'    => ( is => 'ro', isa => 'Str' );
has 'basepath'    => ( is => 'ro', isa => 'Str' );
has 'bname'       => ( is => 'rw', isa => 'Str' );  # Can't call it basename...

has 'extention'   => ( is => 'rw', isa => 'Str' );
has 'ext_type'    => ( is => 'rw', isa => 'Str' );
has 'ext_tar'     => ( is => 'rw', isa => 'Bool' );
has 'ext_cmd'     => ( is => 'rw', isa => 'HashRef' );

has 'mime_raw'    => ( is => 'rw', isa => 'Str' );
has 'mime_type'   => ( is => 'rw', isa => 'Str' );
has 'mime_tar'    => ( is => 'rw', isa => 'Bool' );
has 'mime_cmd'    => ( is => 'rw', isa => 'HashRef' );

has 'likely_type' => ( is => 'rw', isa => 'Str' );
has 'likely_tar'  => ( is => 'rw', isa => 'Bool' );
has 'likely_cmd'  => ( is => 'rw', isa => 'HashRef' );

###############################################################################


around BUILDARGS => sub
{
    my $orig  = shift;
    my $class = shift;

    if ( @_ == 1 && !ref( $_[0] ) ) {
        my $filename = $_[0];
        confess("File doesn't exist.") unless ( -e $filename );
        croak("Error: File is a directory.") if ( -d $filename );

        my $fullpath  = realpath($filename);
        my $basepath  = dirname($fullpath);
        my $extention = $filename =~ s/.*\.(.*)/$1/r;

        return $class->$orig(
            'filename'  => $filename,
            'fullpath'  => $fullpath,
            'basepath'  => $basepath,
            'extention' => $extention,
        );
    }
    else {
        return $class->$orig(@_);
    }
};


sub analysis($self)
{
    $self->extention_analysis();
    $self->mimetype_analysis();
    $self->finalize_analysis();
}


###############################################################################


sub extention_analysis($self)
{
    my ( $ext_tar, $bname );

    # Some initial naive checks for the filetype based on its extention
    if ( $self->filename =~ /\.tar\..*/ ) {
        $self->ext_tar( true );
        $self->bname( $self->filename =~ s/(.*)\.tar\..*/$1/r );
    }
    else {
        my ( $ext_tar, $extention ) = __check_short_tar($self->extention);
        $self->ext_tar( $ext_tar );
        $self->extention( $extention );
        $self->bname( $self->filename =~ s/(.*)\..*/$1/r );
    }

    $self->ext_type( __normalize_type($self->extention) );
    $self->ext_cmd( $self->determine_decompressor( $self->ext_type ) );
}


sub __check_short_tar($extention)
{
    my $ret = '';
    for ($extention) {
        if    (/^(tgz)$/n)          { $ret = 'gz' }
        elsif (/^(tbz|tb2|tbz2)$/n) { $ret = 'bz2' }
        elsif (/^(txz)$/n)          { $ret = 'xz' }
        elsif (/^(tZ|ta[zZ])$/n)    { $ret = 'Z' }
        elsif (/^(tlz)$/n)          { $ret = 'lzma' }
        else                        { return ( false, $extention ) }
    }
    return ( true, $ret );
}


###############################################################################


sub mimetype_analysis($self)
{
    #my $unpack = File::Unpack->new();
    #my $m = $unpack->mime(file => $self->fullpath);
    #my $app = $m->[0];

    my $filename = $self->fullpath;
    my $app = `file --mime-type '$filename'` or croak "$! - $?";
    chomp $app;
    $app = lc $app;

    unless ( $app =~ /application/ ) {
        carp("Not a recognized mime archive of any kind!")
    }

    $app =~ s/.*?x-(.*)/$1/;
    if ( $app =~ /\+/ ) {
        my @progs = split /\+/, $app;
        foreach (@progs) {
            if ( /tar/ ) { $self->mime_tar( true ) }
            else         { $self->mime_raw( $_ ) }
        }
    }
    else {
        my @args = split / /, $app;
        my $type = $args[0];
        unless ( $type ) { carp("Failed to identify mime_raw!") }
        $self->mime_raw( $type );
        $self->mime_tar( false );
    }

    my $tmp  = lc( $self->mime_raw =~ s/-compressed//ri );
    my $type = __normalize_type( $tmp );
    $type    = ( $type ) ? $type : $tmp;

    $self->mime_type( $type );
    $self->mime_cmd( $self->determine_decompressor( $self->mime_type ) );
}


###############################################################################


sub finalize_analysis($self)
{
    #unless ( $self->ext_type eq $self->mime_type ) {
    #    carp("Mismatch between extention and mime_raw analysis!")
    #}

    if    ( $self->mime_type ) { $self->likely_type( $self->mime_type ) }
    elsif ( $self->ext_type )  { $self->likely_type( $self->ext_type ) }
    else                       { confess "No type identified." }

    $self->likely_tar( $self->ext_tar or $self->mime_tar );
    $self->likely_cmd( $self->determine_decompressor( $self->likely_type ) );
}


###############################################################################


sub __normalize_type($extention)
{
    my $type = '';
    $_ = $extention;

    if    (/^(z|Z)$/n)          { $type = 'compress' }
    elsif (/^(gz|bzip)$/n)      { $type = 'gzip' }
    elsif (/^(bz|bz2|bzip2)$/n) { $type = 'bzip2' }
    elsif (/^(xz|lzma|lz)$/n)   { $type = 'xz' }
    elsif (/^(lz4)$/n)          { $type = 'lz4' }
    elsif (/^(tar|cpio)$/n)     { $type = 'tar' }
    elsif (/^(7z|7zip|7-zip)/n) { $type = '7zip' }
    elsif (/^(zpaq)$/n)         { $type = 'zpaq' }
    elsif (/^(zip)$/n)          { $type = 'zip' }
    elsif (/^(arc)$/n)          { $type = 'arc' }
    elsif (/^(ace|winace)$/n)   { $type = 'ace' }
    elsif (/^(rar)$/n)          { $type = 'rar' }

    return $type;
}


###############################################################################


sub determine_decompressor($self, $type)
{
    my ( $CMD, $TFlags, $EFlags, $Stdout );
    my $v7z = '-bso0 -bsp0';
    $TFlags = $EFlags = '';

    $_ = $type;

    if ( /^(z|Z|compress)$/n and which('uncompress') ) {
        $CMD    = 'uncompress';
        $TFlags = $EFlags = '-c';
        $Stdout = true;
    }
    elsif ( /^(gz|z|Z|gzip|compress)$/n and which('gzip') ) {
        $CMD    = 'gzip';
        $TFlags = $EFlags = '-dc';
        $Stdout = true;
    }
    elsif ( /^(bz|bz2|bzip[2]{0,1})$/n and which('bzip2') ) {
        $CMD    = 'bzip2';
        $TFlags = $EFlags = '-dc';
        $Stdout = true;
    }
    elsif ( /^(xz|lzma|lz)$/n and which('xz') ) {
        $CMD    = 'xz';
        $TFlags = $EFlags = '-dc';
        $Stdout = true;
    }
    elsif ( /^(lz4)$/n and which('lz4') ) {
        $CMD    = 'lz4';
        $TFlags = $EFlags = '-dc';
        $Stdout = true;
    }
    elsif ( /^(tar|cpio)$/n ) {
        $CMD    = 'TAR';
        $TFlags = '-xf -- -O';
        $EFlags = '-xf';
    }
    elsif ( /^(7z|gz|bz|bz2|xz|lzma|lz|lz4|zip|cpio|rar|z|Z|jar|
               deb|rpm|a|ar|iso|img|0{1,2}[1-9]|
               compress|gzip|bzip2|7[-]{0,1}zip)$/nx
            and which('7z') )
    {
        $CMD    = '7z';
        $TFlags = "$v7z -so x";
        $EFlags = "$v7z x";
    }
    elsif ( /^(zpaq)$/n and which('zpaq') ) {
        $CMD    = 'zpaq';
        $TFlags = 'NOTAR';
        $EFlags = 'x -- -to tmp >/dev/null';
    }
    elsif ( /^(zip)$/n and which('unzip') ) {
        $CMD    = 'unzip';
        $TFlags = '-p';
    }
    elsif ( /^(arc)$/n and which('arc') ) {
        $CMD    = 'arc';
        $TFlags = 'p';
        $EFlags = 'x';
    }
    elsif ( /^(ace|winace)$/n and which('unace') ) {
        $CMD    = 'unace';
        $TFlags = $EFlags = 'x';
    }
    elsif ( /^(rar)$/n and which('unrar') ) {
        $CMD    = 'unrar';
        $TFlags = $EFlags = 'x';
    }

    return {
        CMD    => $CMD,
        tflags => $TFlags,
        eflags => $EFlags,
        stdout => $Stdout
    };
}


###############################################################################

no Moose;
__PACKAGE__->meta->make_immutable;
