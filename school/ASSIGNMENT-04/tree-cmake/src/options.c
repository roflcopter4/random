#include "tree.h"
#include <getopt.h>
#include <stdlib.h>

static void usage(int status);

enum { DUMMY_CODE = 129 };

static struct option const long_options[] = {
        {"help",       no_argument,       0, 'h'},
        {"version",    no_argument,       0, 'V'},
        {"verbose",    no_argument,       0, 'v'},
        {0, 0, 0, 0}
};


void
decode_switches(int argc, char **argv)
{
        int ch;
#if 0
        opt_flag_ind = 0;
        g_maxsize  = MAXSIZE;
        g_numitems = NUM;
        verbose = loud = use_intlist = false;
#endif

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
");
        exit(status);
}
