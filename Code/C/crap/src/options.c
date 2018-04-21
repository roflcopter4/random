#include "neotags.h"
#include <getopt.h>
#include <stdlib.h>

static void usage(int status);

enum { DUMMY_CODE = 129 };

static const struct option long_options[] = {
        {"help",    no_argument, 0, 'h'},
        {"version", no_argument, 0, 'V'},
        {0, 0, 0, 0}
};


void
handle_options(int argc, char **argv)
{
        int ch;
        if (argc == 1)
                usage(1);

        while ((ch = getopt_long(argc, argv, "hV",
                                 long_options, NULL)) != EOF) {
        switch (ch) {
        case 'V':
                printf("%s %s\n", NAME, VERSION);
                exit(0);
        case 'h':
                usage(0);
                break;
        default:
                usage(2);
        } }
}


static void
usage(int status)
{
        printf("Usage: %s [OPTIONS] <initial size>\n", program_name);
        fputs("\n\
Initial size should be an integer greater than 2.\n\n\
Options:\n\
  -h, --help          Display this help and exit\n\
  -V, --version       Output version information and exit\n\
  -v, --verbose       Display verbose output (hurr)\n\
  -q, --quiet         Do not dispay the tree after processing it\n\
                      (useful for benchmarking)\n\
  -l, --leaf          Only print 'MIN' or 'MAX' for leaf nodes\n\
", stdout);
        exit(status);
}
