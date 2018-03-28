#include "mergesort.h"
#include <sodium.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

/*#define INTLIST {10, 9, 8, 7, 6, 5, 4, 3, 2, 1}*/

#ifndef INTLIST
static void timesort(uint32_t *data);
#endif

#define my_mergesort(LST,LEN)                            \
        do {                                             \
                uint32_t *tmp = merge_sort((LST),(LEN)); \
                free((LST));                             \
                (LST) = tmp;                             \
        } while (0)


#define __my_mergesort(LST,LEN)                            \
        do {                                               \
                uint32_t *tmp = __merge_sort((LST),(LEN)); \
                free((LST));                               \
                (LST) = tmp;                               \
        } while (0)


int
main(int argc, char **argv)
{
        program_name = argv[0];
        decode_switches(argc, argv);


#ifdef INTLIST
        uint32_t tmp[] = INTLIST;
        numitems = sizeof(tmp) / sizeof(uint32_t);
        uint32_t *data = xmalloc(sizeof(tmp));
        memcpy(data, tmp, sizeof(tmp));

        __my_mergesort(data, numitems);
        lazy_printlist(data);
#else
        if (sodium_init() != 0)
                exit(255);
        uint32_t *data = getlist();
        timesort(data);
#endif


        free(data);
        return 0;
}


#ifndef INTLIST
static void
timesort(uint32_t *data)
{
        struct timeval tv1, tv2;
        char msg[1024];

        for (int i = 0; i < opt_flag_ind; ++i) {
                uint32_t *data_copy = cpylist(data);

                if (loud)
                        lazy_printlist(data_copy);

                gettimeofday(&tv1, NULL);

                switch (optflags[i]) {
                case 'Q':
                        puts("Running C's 'qsort'.");
                        lazysort(data_copy, numitems);
                        break;
                case 'b':
                        puts("Running braindead bubblesort.");
                        bubble_sort(data_copy, numitems);
                        break;
                case 's':
                        puts("Running recursive selection sort.");
                        recursive_ss(data_copy, numitems);
                        break;
                case 'm':
                        puts("Trying mergesort.");
                        my_mergesort(data_copy, numitems);
                        break;
                }

                gettimeofday(&tv2, NULL);
                sprintf(msg, "Total time = %f seconds\n\n",
                        (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 +
                        (double) (tv2.tv_sec  - tv1.tv_sec));

                if (verbose) {
                        lazy_printlist(data_copy);
                        puts("");
                }

                printf("%s", msg);

                free(data_copy);
        }
}
#endif
