#include "sort.h"
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

static int compare(const void *a, const void *b);
static void lazysort(uint32_t *list, int size);

static uint32_t * getlist();
static uint32_t * cpylist(uint32_t *list);
static inline int numdigits(int number);

#define NUM 10000
#define MAX 999999

int num_qsort_ops;


int
main(int argc, char **argv)
{
        program_name = argv[0];
        opt_flag_ind = num_qsort_ops = verbose = loud = 0;
        numitems = NUM;
        maxsize  = MAX;
        decode_switches(argc, argv);
        intlen = numdigits(maxsize);

        int i;
        if ((i = sodium_init()) != 0) {
                eprintf("WTF\n");
                exit(i);
        }

        struct timeval tv1, tv2;
        uint32_t *list = getlist();

        for (i = 0; i < opt_flag_ind; ++i) {
                uint32_t *list_copy = cpylist(list);
                gettimeofday(&tv1, NULL);

                if (loud)
                        print_intlist(list_copy, numitems);

                switch (optflags[i]) {
                case 'Q':
                        puts("Running C's 'qsort'.");
                        lazysort(list_copy, numitems);
                        break;
                case 'b':
                        puts("Running braindead bubblesort.");
                        bubble_sort(list_copy, numitems);
                        break;
                case 's':
                        puts("Running selection sort.");
                        selection_sort(list_copy, numitems);
                        break;
                case 'r':
                        puts("Running recursive selection sort.");
                        recursive_ss(list_copy, numitems);
                        break;
                }

                if (verbose) {
                        print_intlist(list_copy, numitems);
                        puts("");
                }

                gettimeofday(&tv2, NULL);
                printf ("Total time = %f seconds\n\n",
                        (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 +
                        (double) (tv2.tv_sec  - tv1.tv_sec));

                free(list_copy);
        }

        free(list);
        return 0;
}


void
print_intlist(uint32_t *intlist, int size)
{
        for (int i = 0; i < size; ++i) {
                if (i < (size - 1))
                        printf("%*u, ", intlen, intlist[i]);
                else
                        printf("%*u\n", intlen, intlist[i]);
        }
        if (numitems > 40)
                printf("\n\n");
}


static uint32_t *
getlist()
{
        uint32_t *list = die_malloc(numitems * sizeof(uint32_t));
        for (int i = 0; i < numitems; ++i)
                list[i] = randombytes_uniform(maxsize);

        return list;
}


static uint32_t *
cpylist(uint32_t *list)
{
        uint32_t *list_copy = die_malloc(numitems * sizeof(uint32_t));
        memcpy(list_copy, list, numitems * sizeof(uint32_t));

        return list_copy;
}


static inline int
numdigits(int number)
{
        char tmp[BUFSIZ];
        snprintf(tmp, BUFSIZ, "%d", number);
        return strlen(tmp);
}


static int
compare(const void *a, const void *b)
{
        /*++num_qsort_ops;*/
        return (*(const uint32_t *)a > *(const uint32_t *)b);
}


static void
lazysort(uint32_t *list, int size)
{
        qsort(list, size, sizeof(uint32_t), compare);
        /*printf("Number of steps taken: %d\n", num_qsort_ops);*/
}
