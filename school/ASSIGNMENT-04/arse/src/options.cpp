#include "arse.h"
#include <cstdio>
#include <cstdlib>
#include <getopt.h>

#include <iostream>
using std::cout;
using std::endl;

static void usage(int status);


char *program_name;
bool quiet, LEAF_ONLY;
enum { DUMMY_CODE = 129 };

static const struct option long_options[] = {
    {"help",    no_argument, nullptr, 'h'},
    {"version", no_argument, nullptr, 'V'},
    {"quiet",   no_argument, nullptr, 'q'},
    {"leaf",    no_argument, nullptr, 'l'},
    {nullptr, 0, nullptr, 0}
};


void
handle_options(int argc, char **argv)
{
        int ch;
        program_name = argv[0];
        if (argc == 1)
                usage(1);

        while ((ch = getopt_long(argc, argv, "hVql",
                                 long_options, NULL)) != EOF) {
        switch (ch) {
        case 'V':
                //printf("%s %s\n", NAME, VERSION);
                cout << NAME " " VERSION << endl;
                exit(0);
        case 'h':
                usage(0);
                break;
        case 'q':
                quiet = true;
                break;
        case 'l':
                LEAF_ONLY = true;
                break;
        default:
                usage(2);
        } }
}


static void
usage(int status)
{
        //printf("Usage: %s [OPTIONS] <initial size>\n", program_name);
        cout << "Usage: " << program_name << " [OPTIONS] <initial size>\n";
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
