#include "students.h"
#include <bsd/bsd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>


int
main (int argc, char **argv)
{
        program_name = argv[0];
        int optind = decode_switches (argc, argv);

        exit (0);
}
