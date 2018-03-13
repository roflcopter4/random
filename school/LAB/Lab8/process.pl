#!/usr/bin/env perl
use strict; use warnings; use 5.26.1;
#use feature 'signatures';
use Data::Dumper qw( Dumper );
use List::Util qw( sum );
use List::MoreUtils qw( minmax );


###############################################################################


our %HTML = (
        'head' => '<header>'
        );


###############################################################################


sub process_file {
    my $filename = shift or die "no filename";
    my(@students, @grades);

    open('fh', '<', $filename) or die;
    foreach (<fh>) {
        next if m/^$/; chomp;
        my ($student, $num) = split;
        push @students, $student;
        push @grades, int($num);
    }
    close('fh');
    return(\@students, \@grades);
}


sub html {
    my $start = shift;
    my $str = shift;
    if ($start) {
        return "<${str}>";
    } else {
        return "</${str}>";
    }
}


sub init_html {
    our @hstack;
}


sub htmladd {
    my $str = shift;

}


###############################################################################

our @hstack = ();

my @data = process_file "input.txt";
my @students = $data[0];
my @grades = $data[1];

init_html();
push(@hstack, html(1, 'html'), "");
print(Dumper \@hstack);


#my $num = scalar(@grades);

#my ($min, $max) = minmax @grades;
#my $mean = sum(@grades) / $num;

#sub html_section {
    #return;
#}

#my $tmp = 23 * 2;
#print << "EOF";
#<html>
#<head>
    #<title>
        #My output chart.
    #</title>
#</head>

#<body>
    #<table>
        #<tr>
            #<td valign="bottom">
                #<div style="width:10px;height: ${tmp}px ;background:blue; border:1px solid red;">
                #</div>
            #</td>
        #</tr>
    #</table>
#</body>
#</html>
#EOF
