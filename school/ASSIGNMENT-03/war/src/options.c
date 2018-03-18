#include <getopt.h>
#include <stdlib.h>

#include "war.h"

static void usage(int status);


/* Option flags and variables */
static struct option const long_options[] = {
        {"help", no_argument, 0, 'h'},
        {"version", no_argument, 0, 'V'},
        {"numcards", required_argument, 0, 'n'},
        {NULL, 0, NULL, 0}
};


/* Set all the option flags according to the switches specified.
   Return the index of the first non-option argument.  */
int
decode_switches(int argc, char **argv, int *numcards)
{
        int c;
        while ((c = getopt_long(argc, argv, "hVn:123",\
                                long_options, NULL)) != EOF)
        { switch (c) {
        case 'V':
                printf("war %s\n", VERSION);
                exit(EXIT_SUCCESS);
        case 'h':
                usage(EXIT_SUCCESS);
        case 'n':
                if ((*numcards = atoi(optarg)) < 1 || *numcards > 3) {
                        fprintf(stderr, "Argument to numcards must be an "
                                "integer between 1 and 3 (inclusive).\n");
                        exit(EXIT_FAILURE);
                }
                break;
        case '1':
        case '2':
        case '3':
                *numcards = atoi((char *)&c);
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
  -h, --help              display this help and exit\n\
  -V, --version           output version information and exit\n\
  -n, --numcards          number of cards placed per turn on the table (1-3)\n\
  -1, -2, -3              aliases for -n1, -n2, and -n3\n\
");
        exit(status);
}
