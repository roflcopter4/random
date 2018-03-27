#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

typedef unsigned int uint32_t;
struct ms_ptr_list {
        uint32_t **list;
        int increment;
        int max;
        int counter;
};
int numitems;

uint32_t * merge_sort(uint32_t *data, int size);
static uint32_t * do_merge_sort(uint32_t *data, int size, struct ms_ptr_list *ptrlist);
static uint32_t * merge(const uint32_t *left, const uint32_t *right, int left_len, int right_len, struct ms_ptr_list *ptrlist);
static void print_intlist(uint32_t *intlist, int size);


#define INTLIST {10, 9, 8, 7, 6, 5, 4, 3, 2, 1}

#define my_mergesort(LST,LEN)                            \
        do {                                             \
                uint32_t *tmp = merge_sort((LST),(LEN)); \
                free((LST));                             \
                (LST) = tmp;                             \
        } while (0)


int
main(int argc, char **argv)
{
        uint32_t tmp[] = INTLIST;
        numitems = sizeof(tmp) / sizeof(uint32_t);
        uint32_t *data = malloc(sizeof(tmp));
        memcpy(data, tmp, sizeof(tmp));

        my_mergesort(data, numitems);
        print_intlist(data, numitems);

        free(data);
        return 0;
}

uint32_t *
merge_sort(uint32_t *data, int size)
{
        struct ms_ptr_list ptrlist;
        ptrlist.max       = size * log(size);
        /*ptrlist.max       = size * size;*/
        ptrlist.increment = ptrlist.max / 2;
        ptrlist.list      = malloc(ptrlist.max * sizeof(uint32_t *));
        ptrlist.counter   = 0;

        uint32_t *ret = do_merge_sort(data, size, &ptrlist);

        for (int i = 0; i < ptrlist.counter - 1; ++i)
                free(ptrlist.list[i]);
        free(ptrlist.list);

        return ret;
}


static void
print_intlist(uint32_t *intlist, int size)
{
        for (int i = 0; i < size; ++i) {
                if (i < (size - 1))
                        printf("%u, ", intlist[i]);
                else
                        printf("%u", intlist[i]);
        }
        puts("");
}


static uint32_t *
do_merge_sort(uint32_t *data, int size, struct ms_ptr_list *ptrlist)
{
        if (size < 2)
                return data;

        int left_len  = size / 2;
        int right_len = size - left_len;

        uint32_t *left   = do_merge_sort(data, left_len, ptrlist);
        uint32_t *right  = do_merge_sort(&data[left_len], right_len, ptrlist);
        uint32_t *result = merge(left, right, left_len, right_len, ptrlist);

        return result;
}


static uint32_t *
merge(const uint32_t *left,
      const uint32_t *right,
      const int left_len,
      const int right_len,
      struct ms_ptr_list *ptrlist)
{
        int lc = 0, rc = 0, i = 0;
        int totalsize    = left_len + right_len;
        uint32_t *result = malloc(totalsize * sizeof(uint32_t));

        if (ptrlist->counter == ptrlist->max) {
                ptrlist->max += ptrlist->increment;
                ptrlist->list = realloc(ptrlist->list,
                                        ptrlist->max * sizeof(uint32_t *));
        }
        ptrlist->list[ptrlist->counter++] = result;

        while (lc < left_len && rc < right_len)
                result[i++] = (left[lc]<=right[rc]) ? left[lc++] : right[rc++];

        while (lc < left_len)
                result[i++] = left[lc++];
        while (rc < right_len)
                result[i++] = right[rc++];

        return result;
}
