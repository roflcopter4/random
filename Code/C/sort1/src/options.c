#include "sort.h"
#include <getopt.h>
#include <stdlib.h>

static void usage(int status);

/* Option flags and variables */
static struct option const long_options[] = {
        {"help",         no_argument,       0, 'h'},
        {"version",      no_argument,       0, 'V'},
        {"num",          required_argument, 0, 'N'},
        {"maxsize",      required_argument, 0, 'M'},
#if 0
        {"verbose",      no_argument,       0, 'v'},
        {"loud",         no_argument,       0, 'L'},
        {"bubble",       no_argument,       0, 'b'},
        {"qsort",        no_argument,       0, 'Q'},
        {"selection",    no_argument,       0, 's'},
        {"recursive_ss", no_argument,       0, 'r'},
#endif
        {NULL, 0, NULL, 0}
};



int
decode_switches(int argc, char **argv)
{
        int c;
#if 0
        while ((c = getopt_long(argc, argv, "hVvLN:M:bQsr",
                                long_options, NULL)) != EOF)
#else
        while ((c = getopt_long(argc, argv, "hVN:M:",
                                long_options, NULL)) != EOF)
#endif
        { switch (c) {
        case 'V':
                printf("sort %s\n", VERSION);
                exit(EXIT_SUCCESS);
        case 'h':
                usage(EXIT_SUCCESS);
                break;
        case 'N':
                numitems = xatoi(optarg);
                break;
        case 'M':
                maxsize = xatoi(optarg);
                break;
#if 0
        case 'v':
                verbose = true;
                break;
        case 'L':
                verbose = loud = true;
                break;
        case 'b':
        case 'Q':
        case 's':
        case 'r':
                optflags[opt_flag_ind++] = (char)c;
                break;
#endif
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
");
        exit(status);
}
