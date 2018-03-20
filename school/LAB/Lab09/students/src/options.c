#include "students.h"
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




int
decode_switches(int argc, char **argv)
{
        int c;


        while ((c = getopt_long(argc,
                                argv,
                                "h"  /* help */
                                "V", /* version */
                                long_options,
                                (int *)0)) != EOF) {
                switch (c) {
                case 'V':
                        printf("students %s\n", VERSION);
                        exit(0);
                case 'h':
                        usage(0);
                        break;
                default:
                        usage(EXIT_FAILURE);
                }
        }

        return optind;
}


static void
usage(int status)
{
        printf("%s - \
kill them\n",
               program_name);
        printf("Usage: %s [OPTION]... [FILE]...\n", program_name);
        printf("\
Options:\n\
  -h, --help                 display this help and exit\n\
  -V, --version              output version information and exit\n\
");
        exit(status);
}


/* Option flags and variables */
/*static struct option const long_options[] = {*/
/*{"help",    no_argument, 0, 'h'},*/
/*{"version", no_argument, 0, 'V'},*/
/*{NULL, 0, NULL, 0}*/
/*};*/


/* Set all the option flags according to the switches specified.
   Return the index of the first non-option argument.  */
/*int*/
/*decode_switches(int argc, char **argv)*/
/*{*/
/*int c;*/
/*while ((c = getopt_long(argc, argv, "hV",*/
/*long_options, NULL)) != EOF)*/
/*{ switch (c) {*/
/*case 'V':*/
/*printf("war %s\n", VERSION);*/
/*exit(EXIT_SUCCESS);*/
/*case 'h':*/
/*usage(EXIT_SUCCESS);*/
/*break;*/
/*default:*/
/*usage(EXIT_FAILURE);*/
/*} }*/

/*return optind;*/
/*}*/


/*static void  [> Display usage info and exit. <]*/
/*usage(int status)*/
/*{*/
/*printf("Usage: %s [OPTION]... [FILE]...\n", program_name);*/
/*printf("\*/
/*Options:\n\*/
/*-h, --help              display this help and exit\n\*/
/*-V, --version           output version information and exit\n\*/
/*");*/
/*exit(status);*/
/*}*/
