#include "sort.h"
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>


int
main(int argc, char **argv)
{
        program_name = argv[0];
        numitems = NUM;
        maxsize  = MAX;
        decode_switches(argc, argv);
        intlen = numdigits(maxsize);
        if (sodium_init() != 0) {
                eprintf("WTF\n");
                exit(1);
        }
        uint32_t *list = getlist();
        /*struct timeval tv1, tv2;*/

        free(list);
        return 0;
}


