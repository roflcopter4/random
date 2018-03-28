#include "mergesort.h"
#include <getopt.h>
#include <stdlib.h>

static void usage(int status);

enum { DUMMY_CODE = 129 };

static struct option const long_options[] = {
        {"help",      no_argument,       0, 'h'},
        {"version",   no_argument,       0, 'V'},
        {"verbose",   no_argument,       0, 'v'},
        {"loud",      no_argument,       0, 'L'},
        {"qsort",     no_argument,       0, 'Q'},
        {"mergesort", no_argument,       0, 'm'},
        {"bubble",    no_argument,       0, 'b'},
        {"selection", no_argument,       0, 's'},
        {"num",       required_argument, 0, 'N'},
        {"maxsize",   required_argument, 0, 'M'},
        {NULL, 0, NULL, 0}
};


void
decode_switches(int argc, char **argv)
{
        int ch;
        opt_flag_ind = 0;
        maxsize  = MAXSIZE;
        numitems = NUM;

        if (argc == 1)
                usage(1);

        while ((ch = getopt_long(argc, argv, "hVvLQmbsN:M:",
                                 long_options, (int *)0)) != EOF) {
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
                numitems = xatoi(optarg);
                break;
        case 'M':
                maxsize = xatoi(optarg);
                break;
        case 'Q':
        case 'm':
        case 'b':
        case 's':
                optflags[opt_flag_ind++] = (char)ch;
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
");
        exit(status);
}
