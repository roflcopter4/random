#include "mergesort.h"
#include <stdlib.h>
#include <string.h>


#if 0
static int64_t do_merge_sort2(uint *data, uint *copy, uint left, uint right);
static int64_t merge2(uint *copy, uint *data, uint left, uint mid, uint right);


int64_t
__merge_sort(uint *data, int64 size)
{
        /*uint *copy = xcalloc(size, sizeof(uint));*/
        uint copy[size];
        int64_t inv_count = do_merge_sort2(data, copy, 0, size-1);
        /*free(copy);*/
        return inv_count;
}


static int64_t
do_merge_sort2(uint *data, uint *copy, uint left, uint right)
{
        int64_t inv_count = 0;

        if (right > left) {
                const uint mid = (left + right) / 2;

                inv_count  = do_merge_sort2(data, copy, left, mid);
                inv_count += do_merge_sort2(data, copy, mid+1, right);

                inv_count += merge2(copy, data, left, mid+1, right);
        }

        return inv_count;
}


static int64_t
merge2(uint *copy, uint *data, uint left, uint mid, uint right)
{
        int64_t inv_count = 0;

        uint lc = left;
        uint rc = mid;
        uint i = left;

        while ((lc <= mid-1) && (rc <= right))
                if (data[lc] <= data[rc]) {
                        copy[i++] = data[lc++];
                } else {
                        copy[i++] = data[rc++];
                        inv_count += (int64_t)mid - (int64_t)lc;
                }

        while (lc <= mid-1)
                copy[i++] = data[lc++];
        while (rc <= right)
                copy[i++] = data[rc++];

        for (i = left; i <= right; ++i)
                data[i] = copy[i];

        return inv_count;
}
#endif



static int64_t do_merge_sort2(uint *data, uint *copy, int64 size, uint index);
static int64_t merge2(uint *copy, const uint *left, const uint *right,
                  uint left_len, uint right_len, uint index);


int64_t
__merge_sort(uint *data, int64 size)
{
        uint *copy = xmalloc(size * sizeof(uint));
        int64_t inv_count = do_merge_sort2(data, copy, size, 0);
        free(copy);
        return inv_count;
}


static int64_t
do_merge_sort2(uint *data, uint *copy, const int64 size, const uint index)
{
        if (size < 2)
                return 0L;

        const uint left_len  = size / 2;
        const uint right_len = size - left_len;

        int64_t inv_count;
        inv_count  = do_merge_sort2(data,            copy, left_len,  index);
        inv_count += do_merge_sort2(&data[left_len], copy, right_len, index + left_len);

        inv_count += merge2(copy, data, &data[left_len], left_len, right_len, index);

        for (uint i = 0; i < size; ++i)
                data[i] = copy[i + index];

        return inv_count;
}


static int64_t
merge2(uint *copy,
       const uint *left,
       const uint *right,
       const uint left_len,
       const uint right_len,
       const uint index)
{
        uint lc = 0, rc = 0;
        uint i = index;
        int64_t inv_count = 0;

        int64_t mid = (int64_t)index + (int64_t)left_len;

        while (lc < left_len && rc < right_len) {
                if (left[lc] <= right[rc]) {
                        copy[i++] = left[lc++];
                } else {
                        copy[i++] = right[rc++];
                        inv_count += mid - ((int64_t)lc + (int64_t)index);
                }
        }

        while (lc < left_len)
                copy[i++] = left[lc++];
        while (rc < right_len)
                copy[i++] = right[rc++];

        return inv_count;
}



#if 0
static void do_merge_sort2(uint *data, uint *copy, uint left, uint right);
static void merge2(uint *copy, uint *data, uint left, uint mid, uint right);


int64_t
__merge_sort(uint *data, int64 size)
{
        uint *copy = xcalloc(size, sizeof(uint));
        do_merge_sort2(data, copy, 0, size-1);
        free(copy);
        return 0;
}


static void
do_merge_sort2(uint *data, uint *copy, uint left, uint right)
{
        if (right > left) {
                const uint mid = (left + right) / 2;

                do_merge_sort2(data, copy, left, mid);
                do_merge_sort2(data, copy, mid+1, right);

                merge2(copy, data, left, mid+1, right);
        }
}


static void
merge2(uint *copy, uint *data, uint left, uint mid, uint right)
{
        uint lc = left;
        uint rc = mid;
        uint i = left;

        while ((lc <= mid-1) && (rc <= right))
                copy[i++] = (data[lc] <= data[rc]) ? data[lc++] : data[rc++];

/*                if (data[lc] <= data[rc])*/
                        /*copy[i++] = data[lc++];*/
                /*else*/
                        /*copy[i++] = data[rc++];*/

        while (lc <= mid-1)
                copy[i++] = data[lc++];
        while (rc <= right)
                copy[i++] = data[rc++];

        for (i = left; i <= right; ++i)
                data[i] = copy[i];

}
#endif
