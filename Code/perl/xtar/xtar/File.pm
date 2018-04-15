package xtar::File;
use warnings; use strict; use v5.26;
use Mouse;
use feature 'signatures';
no warnings 'experimental::signatures';

use constant true  => 1;
use constant false => 0;
use constant MAXKIND => 4;
use Carp qw( carp croak cluck confess );
use File::Basename;
use File::Copy qw( mv );
use File::Spec::Functions qw( rel2abs );
use Scalar::Util qw( looks_like_number );
use File::Which;
use Try::Tiny;

use lib "$ENV{HOME}/random/Code/perl/xtar";
use xtar::Colors qw( esayC eprintC );

##############################################################################

has 'Options'     => ( is => 'ro', isa => 'HashRef' );
has 'ID_Failure'  => ( is => 'rw', isa => 'Bool' );

has 'basepath'    => ( is => 'ro', isa => 'Str' );
has 'filename'    => ( is => 'rw', isa => 'Str' );
has 'fullpath'    => ( is => 'rw', isa => 'Str' );
has 'bname'       => ( is => 'rw', isa => 'Str' );

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

    if ( @_ == 2 && !ref( $_[0] ) ) {
        my $filename = $_[0];
        confess("File doesn't exist.") unless ( -e $filename );
        croak("Error: File is a directory.") if ( -d $filename );

        my $fullpath  = rel2abs($filename);
        my $basepath  = dirname($fullpath);
        my $extention = $filename =~ s/.*\.(.*)/$1/r;
        my $Options   = $_[1];

        return $class->$orig(
            'filename'  => $filename,
            'fullpath'  => $fullpath,
            'basepath'  => $basepath,
            'extention' => $extention,
            'Options'   => $Options
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
        my ( $ext_tar, $extention ) = _check_short_tar($self->extention);
        $self->ext_tar( $ext_tar );
        $self->extention( $extention );
        $self->bname( $self->filename =~ s/(.*)\..*/$1/r );
    }

    $self->ext_type( _normalize_type($self->extention) );
    $self->ext_cmd( $self->determine_decompressor($self->ext_type) );
}


sub _check_short_tar($extention)
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
    my @done;
    my $counter = 0;
    my ( $app, $mimekind );

  RESTART:
    ( $app, $mimekind ) = $self->find_mimetype( \$counter );

    while ( looks_like_number($app) and $app == false and $mimekind <= MAXKIND )
    {
        esayC( 'b', "Mimetype number $mimekind failed." ) if $xtar::DEBUG;
        push @done, $mimekind;
        ( $app, $mimekind ) = $self->find_mimetype( \$counter, @done );
    }

    unless ($app) {
        $self->mime_raw('');
        $self->mime_type('');
        $self->mime_tar('');
        return;
    }

    my $orig = $app;
    $app =~ s|.*?/x-(.*)|$1|;

    if ( $app =~ /\+/ ) {
        my @progs = split /\+/, $app;
        foreach (@progs) {
            if   (/tar/) { $self->mime_tar(true) }
            else         { $self->mime_raw($_) }
        }
    }
    elsif ( $orig =~ m/application/ ) {
        if ( $orig =~ m/octet/ ) {
            goto RESTART;
        }
        else {
            $app =~ s/.*application.(\S+).*/$1/;
            $self->mime_raw($app);
            $self->mime_tar(false);
        }
    }
    else {
        my @args = split / /, $app;
        my $type;
        foreach (@args) {
            last if ( $type = _normalize_type($_) );
        }

        unless ($type) { cluck("Failed to identify mime_raw!") }
        $self->mime_raw($type);
        $self->mime_tar(false);
    }

    my $tmp  = lc( $self->mime_raw =~ s/-compressed//ri );
    my $type = _normalize_type($tmp);
    $type = ($type) ? $type : $tmp;

    if ( not $type and $mimekind < MAXKIND ) {
        goto RESTART;
    }

    $self->mime_type($type);
    $self->mime_cmd( $self->determine_decompressor( $self->mime_type ) );

    if ( $type eq 'zpaq' and $self->extention ne 'zpaq' ) {
        self->move_zpaq();
    }
}


sub find_mimetype ( $self, $counter, @skip )
{
    my $filename = $self->fullpath;
    my ( $app, $kind );
    say "Counter is ${$counter}";

    if ( not( grep( /1/, @skip ) and grep( /2/, @skip ) ) ) {
        my $tmp = true;
        say STDERR 'Using File::Unpack' if $xtar::DEBUG;
        ++${$counter};

        $kind = ${$counter};

        try   { require File::Unpack and File::Unpack->import() }
        catch { $tmp = false };

        if ($tmp) {
            my $unpack = File::Unpack->new();
            my $m      = $unpack->mime( file => $self->fullpath );
            my $index  = ( ${$counter} == 1 ) ? 0 : 2;
            $app = $m->[$index];
        }
        else {
            $app = false;
        }
    }
    elsif ( which('file') and not grep( /3/, @skip ) ) {
        $app = `file --mime-type '$filename'` or confess "$! - $?";
        $kind = 3;
        chomp $app;
    }
    elsif ( which('file') and not grep( /4/, @skip ) ) {
        $app = `file '$filename'` or confess "$! - $?";
        $kind = 4;
        chomp $app;
    }
    else {
        eprintC( 'bRED', 'Error: ' );
        print STDERR <<'EOF';
No mimetype tools found. If using a *nix system, check whether the
`file(1)' utility is properly installed (it should be). Otherwise, please
install the package `File::Unpack' from cpan or your local package manager if
available. Will attempt to extract using only file extention information. This
will fail if the extention is not accurate or if the file lacks one entirely.
EOF
        return ( false, 0 );
    }

    $app = lc $app;
    return ( $app, $kind );
}


###############################################################################


sub finalize_analysis($self)
{
    if    ( $self->mime_type ) { $self->likely_type( $self->mime_type ) }
    elsif ( $self->ext_type )  { $self->likely_type( $self->ext_type ) }
    else                       {
        if ( $self->Options->{'force'} ) {
            esayC( 'RED', 'Warning: No type identified.' );
            $self->ID_Failure( true );
            return;
        }
        else {
            esayC( 'bRED', <<'EOF' );
Error: No type identified at all. If you are sure that this is an archive of
some kind, re-run this program with the -f/--force flag to attempt to extract
it with every known program until something works.
EOF
            exit 127;
        }
    }

    $self->likely_tar( $self->ext_tar or $self->mime_tar );
    $self->likely_cmd( $self->determine_decompressor( $self->likely_type ) );
}


###############################################################################


sub _normalize_type($extention)
{
    my $type = '';
    $_ = $extention;

    if    (/^(z|Z)$/ni)          { $type = 'compress' }
    elsif (/^(gz|bzip)$/ni)      { $type = 'gzip' }
    elsif (/^(bz|bz2|bzip2)$/ni) { $type = 'bzip2' }
    elsif (/^(xz|lzma|lz)$/ni)   { $type = 'xz' }
    elsif (/^(lz4)$/ni)          { $type = 'lz4' }
    elsif (/^(tar|cpio)$/ni)     { $type = 'tar' }
    elsif (/^(7z|7zip|7-zip)/ni) { $type = '7zip' }
    elsif (/^(zpaq)$/ni)         { $type = 'zpaq' }
    elsif (/^(zip)$/ni)          { $type = 'zip' }
    elsif (/^(arc)$/ni)          { $type = 'arc' }
    elsif (/^(ace|winace)$/ni)   { $type = 'ace' }
    elsif (/^(rar)$/ni)          { $type = 'rar' }

    return $type;
}


sub move_zqaq($self)
{
    mv( $self->fullpath, $self->fullpath . '.zpaq' );
    $self->fullpath( $self->fullpath . '.zpaq' );
    $self->filename( basename($self->fullpath) );
    $self->extention( 'zpaq' );
}


###############################################################################


sub determine_decompressor($self, $type)
{
    my ( $CMD, $TFlags, $EFlags, $Stdout );

    my $V     = $self->Options->{'verbose'};
    my $v7z   = ( $V ) ? '' : '-bso0 -bsp0';
    my $vzpaq = ( $V ) ? '' : ' >/dev/null';

    $TFlags = $EFlags = '';
    $_ = $type;

    if ( /^(z|compress)$/ni and which('uncompress') ) {
        $CMD    = 'uncompress';
        $TFlags = $EFlags = '-c';
        $Stdout = true;
    }
    elsif ( /^(gz|z|gzip|compress)$/ni and which('gzip') ) {
        $CMD    = 'gzip';
        $TFlags = $EFlags = '-dc';
        $Stdout = true;
    }
    elsif ( /^(bz|bz2|bzip[2]?)$/ni and which('bzip2') ) {
        $CMD    = 'bzip2';
        $TFlags = $EFlags = '-dc';
        $Stdout = true;
    }
    elsif ( /^(xz|lzma|lz)$/ni and which('xz') ) {
        $CMD    = 'xz';
        $TFlags = $EFlags = '-dc';
        $Stdout = true;
    }
    elsif ( /^(lz4)$/ni and which('lz4') ) {
        $CMD    = 'lz4';
        $TFlags = $EFlags = '-dc';
        $Stdout = true;
    }
    elsif ( /^(tar|cpio)$/ni ) {
        $CMD    = 'TAR';
        $TFlags = '-xf -- -O';
        $EFlags = '-xf';
    }
    elsif ( /^(7z|gz|bz|bz2|xz|lzma|lz|lz4|zip|cpio|rar|z|jar|
               deb|rpm|a|ar|iso|img|0{1,2}[1-9]|
               compress|gzip|bzip2|7[-]?zip)$/nxi
            and which('7z') )
    {
        $CMD    = '7z';
        $TFlags = "$v7z -so x";
        $EFlags = "$v7z x";
    }
    elsif ( /^(zpaq)$/ni and which('zpaq') ) {
        $CMD    = 'zpaq';
        $TFlags = 'NOTAR';
        $EFlags = "x -- -to tmp" . $vzpaq;
    }
    elsif ( /^(zip)$/ni and which('unzip') ) {
        $CMD    = 'unzip';
        $TFlags = '-p';
    }
    elsif ( /^(arc)$/ni and which('arc') ) {
        $CMD    = 'arc';
        $TFlags = 'p';
        $EFlags = 'x';
    }
    elsif ( /^(ace|winace)$/ni and which('unace') ) {
        $CMD    = 'unace';
        $TFlags = $EFlags = 'x';
    }
    elsif ( /^(rar)$/ni and which('unrar') ) {
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

no Mouse;
__PACKAGE__->meta->make_immutable;
