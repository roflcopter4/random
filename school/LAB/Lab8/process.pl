#!/usr/bin/env perl
use strict; use warnings; use v5.26;
use feature qw( signatures );
no warnings qw( experimental::signatures );

use Carp            qw( croak carp );
use Data::Dumper    qw( Dumper     );
use List::Util      qw( sum        );
use List::MoreUtils qw( minmax     );

###############################################################################
# Evil globals

our @stack;
our @level_stack;
our $TAB    = ' ' x 4;
our $output = '';
our $level  = 0;

###############################################################################
# File and data processing

sub process_file($filename) {
    my @grade_list;
    open('fh', '<', $filename) or croak $!;

    while (<fh>) {
        next if /^$/;
        chomp;
        my ( $student, $grd ) = split;
        push @grade_list, int($grd);
    }
    close('fh') or carp $!;

    return( sort( { $a <=> $b } @grade_list ) );
}

sub filter_data($grade_list) {
    my @array  = [];
    my $max    = 9;

    foreach my $grade (@{$grade_list}) {
        if ( $grade > $max ) {
            $array[ ++$#array ] = [];
            $max += 10;
            $max = 100 if $max == 99;
        }
        push @{$array[-1]}, $grade;
    }
    return @array;
}

sub print_data($average, $min, $max, $list) {
    print "Average: $average\nMax: $max\nMin: $min\n";
    $_ = join('', map({"[@{$_}]\n"} @{$list}));
    s/(\d+)(?![\]\d])/$1,/gm;
    print;
}

###############################################################################
# HTML generation functions

sub add_content($content) {
    my @stuff = split(/\n/, $content);
    foreach (@stuff) {
        chomp;
        $output .= $TAB x $level . "$_\n";
    }
}

sub check_stack {
    if ( $stack[-1] ) {
        add_content(pop @stack);
        push @stack, '';
    }
}

sub html($op) {
    if ( $op =~ /init/i ) {
        $output = "<html>\n\n";
    } elsif ( $op =~ /end|finish/i ) {
        $output .= "</html>\n";
    } else {
        carp 'Invalid operation.';
    }
}

sub do_add($sec) {
    push @stack, $sec;
    push @stack, '';
    push @level_stack, $level;
}

sub add(@ARGS) {
    check_stack;
    foreach my $sec (@ARGS) {
        do_add $sec;
        $output .= ${TAB} x $level++ . "<${sec}>\n";
    }
}

sub css($sec, $format) {
    check_stack;
    do_add $sec;
    $output .= ${TAB} x $level++ . "<${sec} ${format}>\n";
}

sub end($max = 1) {
    for ( 0 .. --$max ) {
        my $content = pop @stack;
        my $section = pop @stack;

        if ( $content ) {
            add_content $content;
        }
        $level   = pop @level_stack;
        $output .= ${TAB} x ${level} . "</${section}>\n";
        $output .= "\n" unless @level_stack;
    }
}

sub lit($in) {
    $stack[-1] .= $in;
}

sub para($in) {
    #$in =~ s|\n|</br>\n|g;
    $in =~ s/\n/<\/br>\n/g;
    $in = "$in\n" unless $in =~ /\n$/;
    $stack[-1] .= "<p>\n${in}</p>\n";
}


###############################################################################
# Main data processing


my @grade_list     = process_file('input.txt');
my $average        = sum(@grade_list) / scalar(@grade_list);
my ( $min, $max )  = minmax(@grade_list);
my @sectioned_data = filter_data(\@grade_list);

#print_data $average, $min, $max, \@sectioned_data;

# Perl lacks a way to generate a range with a step, so we do this the hard way.
my @ranges;
for ( my $I = 0; $I <= 90; $I += 10 ) {
    my $J = ($I == 90) ? (100) : ($I+9);
    push @ranges, sprintf('[%2d-%-3d]', $I, $J);
}
my $buf = join ' ', @ranges;


###############################################################################
# HTML page generation

html 'init';
add 'body';
    add 'h1';
        lit "This took so ridiculously long it's seriously embarrassing.";
    end;
end;

add 'body';
    para <<~ "EOF";
        Class average: $average
        Highest grade: $max
        Lowest grade : $min
        EOF
    add 'table';
        add 'tr';
            my $i = 13;
            foreach my $section (@sectioned_data) {
                my $len = scalar(@{$section}) * 35;
                css 'td', 'valign="bottom"';
                    lit <<~ "EOF";
                        <div style="width:62px;height: ${len}px ;background:blue; border:1px solid red; writing-mode:tb-rl"></div>
                        EOF
                end;
                css 'td', 'valign="bottom"';
                    lit <<~ "EOF";
                        <div style="width:${i}px;height: ${len}px ;background:white;"></div>
                        EOF
                end;
                ++$i;
            }
        end;
    end;
    css 'font', "face=courier size='3'";
        lit "$buf";
    end;
end;
html 'end';

###############################################################################
# And at long last:

print $output;
