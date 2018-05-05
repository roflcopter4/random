#include "preproc.h"
#include <getopt.h>
#include <stdlib.h>
#include <unistd.h>

static void usage(int status);

enum { DUMMY_CODE = 129 };

static const struct option long_options[] = {
        {"help",    no_argument,       0, 'h'},
        {"version", no_argument,       0, 'V'},
        {"width",   required_argument, 0, 'w'},
        {"header",  no_argument,       0, 'H'},
        {"noskip",  no_argument,       0, 'n'},
        {"all",     no_argument,       0, 'a'},
        {0, 0, 0, 0}
};


bool
handle_options(int argc, char **argv)
{
        int ch;
        WIDTH = DEFAULT_WIDTH;

        while ((ch = getopt_long(argc, argv, "hVw:Hna",
                                 long_options, NULL)) != EOF) {
                switch (ch) {
                case 'V':
                        puts(PACKAGE_STRING);
                        exit(0);
                case 'h':
                        usage(0);
                        break;
                case 'w':
                        WIDTH = xatoi(optarg);
                        break;
                case 'H':
                        IS_HEADER = true;
                        break;
                case 'n':
                        NO_SKIP = true;
                        break;
                case 'a':
                        ALL = true;
                        break;
                default:
                        usage(2);
                }
        }

        return (bool)isatty(0);
}


static void
usage(int status)
{
        printf("Usage: %s [OPTIONS]\n", program_name);
        fputs("\n\
Options:\n\
  -h, --help          Display this help and exit\n\
  -V, --version       Output version information and exit\n\
  -w, --width         Specify the desiged indentation width\n\
  -H, --header        Ignore the first ifdef statement found\n\
  -n, --noskip        Don't skip a level even if the file is a header\n\
  -a, --all           Also indent normal code\n\
", stdout);
        exit(status);
}
