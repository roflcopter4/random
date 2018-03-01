#!/usr/bin/perl
use warnings; use strict;

open('fp', '<', 'rainfall.txt') or die $!;
my @data;
my $cat = 1;

while (<fp>) {
    my @lst = split / /;
    $lst[0] = uc $lst[0];
    push(@data, \@lst);
}
@data = sort({ $a->[1] <=> $b->[1] } @data);

foreach my $line (@data) {
    $line->[1] > $cat+10 and $cat += 10*(int(($line->[1]-$cat)/10)) and printf("[%d - %d]\n", $cat, $cat+9);
    format =
@||||||||||||||||||||||||| @>>>
$line->[0],                sprintf("%.1f", $line->[1])
.
    write;
}
