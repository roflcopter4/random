#!/usr/bin/env perl
use strict; use warnings; use v5.26;
use Carp;
use Data::Dumper;
use File::Unpack;

my $log;
my $mime = File::Unpack->new();

my $file = shift or croak;

my $m = $mime->mime(file => $file);

print Dumper( $log, $mime, $m );
