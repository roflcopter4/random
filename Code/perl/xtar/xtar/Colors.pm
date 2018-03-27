package xtar::Colors;
use warnings; use strict; use v5.26;
use Exporter 'import';
use feature 'signatures';
no warnings 'experimental::signatures';

our @EXPORT_OK = qw( sayC fsayC esayC );

my %color_hash = (
    red     => 31,
    green   => 32,
    yellow  => 33,
    blue    => 34,
    magenta => 35,
    cyan    => 36
);


sub sayC( $color, @strings )
{
    my $str = get_string( $color, @strings );

    say( $str );
}


sub fsayC( $file, $color, @strings )
{
    my $str = get_string( $color, @strings );

    say( $file $str );
}


sub esayC ( $color, @strings )
{
    fsayC( *STDERR, $color, @strings );
}


sub get_string( $color, @strings )
{
    my ( $isbold, $esc );

    $color =~ /^(B{0,1})(.*)/;
    $isbold = $1;
    $color  = $2;

    $esc = get_escape( $color_hash{$color}, $isbold );

    return $esc . "@strings" . "\033[0m";
}


sub get_escape( $num, $isbold )
{
    my $bold = ($isbold) ? 1 : 0;

    return "\033[${bold}m" . "\033[${num}m";
}
