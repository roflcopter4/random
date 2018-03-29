#include "mergesort.h"
#include <getopt.h>
#include <limits.h>
#include <stdlib.h>

#define MAXSIZE UINT_MAX
#define NUM     10000

static void usage(int status);

enum { DUMMY_CODE = 129 };

static struct option const long_options[] = {
        {"help",       no_argument,       0, 'h'},
        {"version",    no_argument,       0, 'V'},
        {"verbose",    no_argument,       0, 'v'},
        {"loud",       no_argument,       0, 'L'},
        {"qsort",      no_argument,       0, 'Q'},
        {"mergesort",  no_argument,       0, 'm'},
        {"invertions", no_argument,       0, 'I'},
        {"bubble",     no_argument,       0, 'b'},
        {"selection",  no_argument,       0, 's'},
        {"num",        required_argument, 0, 'N'},
        {"maxsize",    required_argument, 0, 'M'},
        {"intlist",    required_argument, 0, 'i'},
        {0, 0, 0, 0}
};


void
decode_switches(int argc, char **argv)
{
        int ch;
        opt_flag_ind = 0;
        g_maxsize  = MAXSIZE;
        g_numitems = NUM;
        verbose = loud = use_intlist = false;

        if (argc == 1)
                usage(1);

        while ((ch = getopt_long(argc, argv, "hVvLQmIbsN:M:i",
                                 long_options, NULL)) != EOF) {
        switch (ch) {
        case 'V':
                printf("%s %s\n", NAME, VERSION);
                exit(0);
        case 'h':
                usage(0);
                break;
        case 'v':
                verbose = true;
                break;
        case 'L':
                verbose = loud = true;
                break;
        case 'N':
                g_numitems = xatoi(optarg);
                break;
        case 'M':
                g_maxsize = xatoi(optarg);
                break;
        case 'Q':
        case 'm':
        case 'b':
        case 's':
        case 'I':
                optflags[opt_flag_ind++] = (char)ch;
                break;
        case 'i':
                use_intlist = true;
                break;
        default:
                usage(2);
        } }
}


static void
usage(int status)
{
        printf("Usage: %s [OPTIONS]\n", program_name);
        printf("\n\
Options:\n\
  -h, --help          Display this help and exit\n\
  -V, --version       Output version information and exit\n\
  -v, --verbose       Display verbose output (hurr)\n\
  -L, --loud          Be even more verbose\n\
  -N, --num           Number of randomly generated numbers to sort\n\
  -M, --maxsize       Maximum size of randomly generated numbers\n\
  -Q, --qsort         Sort with C's qsort function\n\
  -m, --mergesort     Sort with my mergesort function\n\
  -b, --bubble        Sort with braindead bubblesort\n\
  -s, --selection     Sort with recursive selection sort\n\
  -I, --invertions    Sort with modified mergesort and print no of invertions\n\
  -i, --intlist       Override normal operation and sort a literal list\n\
");
        exit(status);
}
