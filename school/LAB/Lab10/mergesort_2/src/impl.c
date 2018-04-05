#include "mergesort.h"
#include <stdlib.h>

static void do_merge_sort(uint *data, uint *copy, int64 size, uint index);
static void merge(uint *copy, const uint *left, const uint *right,
                  uint left_len, uint right_len, uint index);


void
merge_sort(uint *data, const int64 size)
{
        uint *copy = xmalloc(size * sizeof(uint));
        do_merge_sort(data, copy, size, 0);
        free(copy);
}


static void
do_merge_sort(uint *data, uint *copy, const int64 size, const uint index)
{
        if (size < 2)
                return;

        const uint left_len  = size / 2;
        const uint right_len = size - left_len;

        do_merge_sort(data,            copy, left_len,  index);
        do_merge_sort(&data[left_len], copy, right_len, index + left_len);

        merge(copy, data, &data[left_len], left_len, right_len, index);

        for (uint i = 0; i < size; ++i)
                data[i] = copy[index + i];
}


static void
merge(uint *copy,
      const uint *left,
      const uint *right,
      const uint left_len,
      const uint right_len,
      const uint index)
{
        uint lc = 0, rc = 0;  /* left counter, right counter */
        uint i = index;

        while (lc < left_len && rc < right_len)
                copy[i++] = (left[lc] <= right[rc]) ? left[lc++] : right[rc++];

        /*while (lc < left_len && rc < right_len)*/
                /*if (left[lc] <= right[rc])*/
                        /*copy[i++] = left[lc++];*/
                /*else*/
                        /*copy[i++] = right[rc++];*/


        while (lc < left_len)
                copy[i++] = left[lc++];
        while (rc < right_len)
                copy[i++] = right[rc++];
}

