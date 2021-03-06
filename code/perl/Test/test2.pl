#!/usr/bin/env perl
use strict; use warnings; use v5.26;
use String::ShellQuote;
use List::Util 'max';
use Inline C => <<'_END_OF_C_';
#include <locale.h>
void my_printf_wrapper(char *fmt, int len, int num, char *str) {
        setlocale(LC_NUMERIC, "");
        printf(fmt, len, num, str);
}
_END_OF_C_

my $input = eval '`du -d1 '.shell_quote(@ARGV).' | sort -nr`' or die "$!";
chomp $input;

my @list    = map { [ split(/\t/, $_) ] } split(/\n/, $input);
my $longest = length max(map { $_->[0] } @list);
$longest   += int($longest / 3);

foreach (@list) {
    my_printf_wrapper("%'*d     %s\n", $longest, @{$_})
}
