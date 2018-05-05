#include "mergesort.h"
#include <sodium.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#define INTLIST {10, 9, 8, 7, 6, 5, 4, 3, 2, 1}
/*#define INTLIST {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}*/
/*#define INTLIST {}*/
/*#define INTLIST {5, 1, 2, 4}*/
/*#define INTLIST {1, 2, 10, 4, 5, 8, 7}*/
#define LISTMAX 10
/*#define LISTMAX 9*/

static void timesort(uint *data, int64 size);


int
main(int argc, char **argv)
{
        program_name = argv[0];
        decode_switches(argc, argv);
        uint *data;

        if (use_intlist) {
                {
                        uint tmp[] = INTLIST;
                        g_numitems = sizeof(tmp) / sizeof(uint);
                        g_maxsize  = LISTMAX;
                        data = xmalloc(sizeof(tmp));
                        memcpy(data, tmp, sizeof(tmp));
                }        

                pretty_print(data, g_numitems);

                long int invertions = count_invertions(data, g_numitems);

                pretty_print(data, g_numitems);
                printf("There were %ld invertions\n", invertions);

        } else {
                if (sodium_init() != 0)
                        xeprintf(255, "How is this even possible.\n");
                data = getlist(g_numitems, g_maxsize);

                if (DO_NOTHING)
                        pretty_print(data, g_numitems);
                else
                        timesort(data, g_numitems);
        }

        free(data);
        return 0;
}


static void
timesort(uint *data, int64 size)
{
        struct timeval tv1, tv2;
        char msg[1024];

        if (loud) {
                puts("Unsorted data.\n");
                pretty_print(data, size);
                puts("\n");
        }

        for (uint16_t i = 0; i < opt_flag_ind; ++i) {
                uint *data_copy = cpylist(data, size);
                gettimeofday(&tv1, NULL);
                long int invertions = -1;

                switch (optflags[i]) {
                case 'Q':
                        puts("Running C's 'qsort'.");
                        lazysort(data_copy, size);
                        break;
                case 'b':
                        puts("Running braindead bubblesort.");
                        bubble_sort(data_copy, size);
                        break;
                case 's':
                        puts("Running recursive selection sort.");
                        recursive_ss(data_copy, size);
                        break;
                case 'm':
                        puts("Trying mergesort.");
                        merge_sort(data_copy, size);
                        break;
                case 'I':
                        puts("Using modified mergesort.");
                        invertions = count_invertions(data_copy, size);
                        break;
                }

                /* I copied this section verbatim from stackexchange... */
                gettimeofday(&tv2, NULL);
                sprintf(msg, "Total time = %f seconds\n",
                        (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 +
                        (double) (tv2.tv_sec  - tv1.tv_sec));

                if (verbose) {
                        putchar('\n');
                        pretty_print(data_copy, size);
                        putchar('\n');
                }

                if (invertions >= 0)
                        printf("There were %ld invertions\n", invertions);
                puts(msg);
                if (verbose && i < opt_flag_ind - 1)
                        putchar('\n');

                free(data_copy);
        }
}

