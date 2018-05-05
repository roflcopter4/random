#!/usr/bin/env perl
use strict; use warnings; use v5.26;
use constant INT_MAX  => 4294967292;
#use constant true  => 1;
#use constant false => 0;
use Math::Random::Secure qw(irand);

use Inline (C => Config =>
    cc          => 'clang',
    optimize    => '-O3 -march=native',
    ccflagsex   => '-Wall -Wextra -std=gnu11 -D_GNU_SOURCE',
    build_noisy => 1,
    force_build => 1
);

use Inline C => <<~'_END_C_';
    #define eprintf(...) fprintf(stderr, __VA_ARGS__)
    #define xeprintf(STATUS, ...)             \
        do {                                  \
                fprintf(stderr, __VA_ARGS__); \
                exit(STATUS);                 \
        } while (0)

    #define QSTYPE       U32
    #define QS_SIZE_TYPE ssize_t

    static int get_twidth();
    static int numdigits(uint number);
    static void do_quick_sort(QSTYPE *data, I64 first, I64 last);
    static I64 partition(QSTYPE *data, I64 first, I64 last);


    void C_pretty_print(int len, ...)
    {
            Inline_Stack_Vars;
            len = Inline_Stack_Items;
            int intlen = numdigits(UINT_MAX);
            int dig_per_line = get_twidth() / (intlen + 2);
            int n = 0;

            setlocale(LC_NUMERIC, "");

            for (int i = 1; i < len; ++i) {
                    SV *tmp = Inline_Stack_Item(i);
                    printf("%'*lu  ", intlen, SvUV(tmp));
                    if (++n == dig_per_line) {
                            putchar('\n');
                            n = 0;
                    }
            }

            puts("\n");
            Inline_Stack_Void;
    }


    void C_my_quick_sort(int len, ...)
    {
            Inline_Stack_Vars;
            len = Inline_Stack_Items - 1;
            U32 *array = calloc(len, sizeof(*array));

            for (int i = 0; i < len; ++i)
                    array[i] = SvUV(Inline_Stack_Item(i+1));

            do_quick_sort(array, 0, (I64)len - 1L);

            Inline_Stack_Reset;

            for (int i = 0; i < len; ++i) {
                    SV *tmp = newSVuv(array[i]);
                    Inline_Stack_Push(tmp);
            }

            free(array);
            Inline_Stack_Return(len);
            Inline_Stack_Done;
    }


    static int get_twidth()
    {
            struct winsize size;
            if (ioctl(0, TIOCGWINSZ, (char *) &size) < 0)
                    xeprintf(45, "TIOCGWINSZ error");
            return size.ws_col;
    }
    static int numdigits(U32 number)
    {
            char tmp[BUFSIZ];
            setlocale(LC_ALL, "");
            snprintf(tmp, BUFSIZ, "%'u", number);
            return strlen(tmp);
    }

    static void do_quick_sort(QSTYPE *data, I64 first, I64 last)
    {
            if (first < last) {
                    I64 pivot = partition(data, first, last);
                    do_quick_sort(data, first, pivot - 1);
                    do_quick_sort(data, pivot + 1, last);
            }
    }
    static I64 partition(QSTYPE *data, I64 first, I64 last)
    {
            QSTYPE pivot_val = data[first];
            I64 left  = first + 1;
            I64 right = last;
            bool done = false;

            while (!done) {
                    while (left <= right && data[left] <= pivot_val)
                            ++left;

                    while (left <= right && data[right] >= pivot_val)
                            --right;

                    if (left > right) {
                            done = true;
                    } else {
                            QSTYPE temp = data[left];
                            data[left]  = data[right];
                            data[right] = temp;
                    }
            }

            QSTYPE temp = data[first];
            data[first] = data[right];
            data[right] = temp;
            return right;
    }
    _END_C_

sub pretty_print  { C_pretty_print(0, @_) }
sub my_quick_sort { C_my_quick_sort(0, @_) }


my @random;
for (0 .. 100) {
    push @random, irand(INT_MAX);
    # push @random, int(rand(INT_MAX));
}

pretty_print @random;

my @arse = my_quick_sort @random;

pretty_print @arse;

my @butt = sort { $a <=> $b } @random;

pretty_print @butt;
