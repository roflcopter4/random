#include "sort.h"
#include <getopt.h>
#include <stdlib.h>

static void usage(int status);

/* Option flags and variables */
static struct option const long_options[] = {
        {"help",         no_argument,       0, 'h'},
        {"version",      no_argument,       0, 'V'},
        {"verbose",      no_argument,       0, 'v'},
        {"loud",         no_argument,       0, 'L'},
        {"num",          required_argument, 0, 'N'},
        {"maxsize",      required_argument, 0, 'M'},
        {"bubble",       no_argument,       0, 'b'},
        {"qsort",        no_argument,       0, 'Q'},
        {"selection",    no_argument,       0, 's'},
        {"recursive_ss", no_argument,       0, 'r'},
        {NULL, 0, NULL, 0}
};


static int
xatio(char *str)
{
        char *endptr;
        int num = strtol(str, &endptr, 10);

        if (endptr == str) {
                eprintf("Invalid integer '%s'.\n", str);
                exit(2);
        }

        return num;
}


int
decode_switches(int argc, char **argv)
{
        int c;
        while ((c = getopt_long(argc, argv, "hVvLN:M:bQsr",
                                long_options, NULL)) != EOF)
        { switch (c) {
        case 'V':
                printf("sort %s\n", VERSION);
                exit(EXIT_SUCCESS);
        case 'h':
                usage(EXIT_SUCCESS);
                break;
        case 'v':
                verbose = true;
                break;
        case 'L':
                verbose = loud = true;
                break;
        case 'N':
                numitems = xatio(optarg);
                break;
        case 'M':
                maxsize = xatio(optarg);
                break;
        case 'b':
        case 'Q':
        case 's':
        case 'r':
                optflags[opt_flag_ind++] = (char)c;
                break;
        default:
                usage(EXIT_FAILURE);
        } }

        return optind;
}


static void  /* Display usage info and exit. */
usage(int status)
{
        printf("Usage: %s [OPTION]... [FILE]...\n", program_name);
        printf("\
Options:\n\
  -h, --help          display this help and exit\n\
  -V, --version       output version information and exit\n\
  -v, --verbose       display verbose output (hurr)\n\
  -b  --bubblesort    run bubblesort\n\
  -Q  --qsort         run C's standard 'qsort'\n\
");
        exit(status);
}
