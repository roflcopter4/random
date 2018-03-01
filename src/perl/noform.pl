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
    if (int($line->[1]) >= $cat+8) {
        $cat += int(($line->[1] - $cat)/10) * 10;
        printf("[%d - %d]\n", $cat, $cat+9);
    }
    printf("\t%-20s%5.1f\n", $line->[0], $line->[1]);
}
