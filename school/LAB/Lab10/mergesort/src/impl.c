#include "mergesort.h"
#include <math.h>
#include <stdlib.h>

static uint32_t *
do_merge_sort(uint32_t *data, int size, struct ms_ptr_list *ptrlist);

static uint32_t *
merge(const uint32_t *left, const uint32_t *right, int left_len, int right_len, struct ms_ptr_list *ptrlist);


uint32_t *
merge_sort(uint32_t *data, int size)
{
        struct ms_ptr_list ptrlist;
        ptrlist.max       = size * log(size);  /* Seems a reasonable guess */
        ptrlist.increment = ptrlist.max / 2;
        ptrlist.list      = xmalloc(ptrlist.max * sizeof(uint32_t *));
        ptrlist.counter   = 0;

        uint32_t *ret = do_merge_sort(data, size, &ptrlist);

        for (int i = 0; i < ptrlist.counter - 1; ++i)
                free(ptrlist.list[i]);
        free(ptrlist.list);

        return ret;
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
        uint32_t *result = xmalloc(totalsize * sizeof(uint32_t));

        if (ptrlist->counter == ptrlist->max) {
                ptrlist->max += ptrlist->increment;
                ptrlist->list = xrealloc(ptrlist->list,
                                         ptrlist->max * sizeof(uint32_t *));
        }
        ptrlist->list[ptrlist->counter++] = result;

        while (lc < left_len && rc < right_len)
                result[i++] = (left[lc] <= right[rc]) ? left[lc++] : right[rc++];

        while (lc < left_len)
                result[i++] = left[lc++];
        while (rc < right_len)
                result[i++] = right[rc++];

        return result;
}


#if 0
        while (lc < left_len && rc < right_len)
                if (left[lc] <= right[rc])
                        result[i++] = left[lc++];
                else
                        result[i++] = right[rc++];
#endif
