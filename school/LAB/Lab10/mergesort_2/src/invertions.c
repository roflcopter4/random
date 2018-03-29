#include "mergesort.h"
#include <stdlib.h>
#include <string.h>

typedef long int Lint;


#if 0
static Lint do_merge_sort2(uint *data, uint *copy, uint left, uint right);
static Lint merge2(uint *copy, uint *data, uint left, uint mid, uint right);


Lint
__merge_sort(uint *data, uint size)
{
        /*uint *copy = xcalloc(size, sizeof(uint));*/
        uint copy[size];
        Lint inv_count = do_merge_sort2(data, copy, 0, size-1);
        /*free(copy);*/
        return inv_count;
}


static Lint
do_merge_sort2(uint *data, uint *copy, uint left, uint right)
{
        Lint inv_count = 0;

        if (right > left) {
                const uint mid = (left + right) / 2;

                inv_count  = do_merge_sort2(data, copy, left, mid);
                inv_count += do_merge_sort2(data, copy, mid+1, right);

                inv_count += merge2(copy, data, left, mid+1, right);
        }

        return inv_count;
}


static Lint
merge2(uint *copy, uint *data, uint left, uint mid, uint right)
{
        Lint inv_count = 0;

        uint lc = left;
        uint rc = mid;
        uint i = left;

        while ((lc <= mid-1) && (rc <= right))
                if (data[lc] <= data[rc]) {
                        copy[i++] = data[lc++];
                } else {
                        copy[i++] = data[rc++];
                        inv_count += (Lint)mid - (Lint)lc;
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



static Lint do_merge_sort2(uint *data, uint *copy, uint size, uint index);
static Lint merge2(uint *copy, const uint *left, const uint *right,
                  uint left_len, uint right_len, uint index);


Lint
__merge_sort(uint *data, uint size)
{
        /*uint *copy = xcalloc(size, sizeof(uint));*/
        uint *copy = xmalloc(size * sizeof(uint));
        /*uint copy[size];*/
        Lint inv_count = do_merge_sort2(data, copy, size, 0);
        free(copy);
        return inv_count;
}


static Lint
do_merge_sort2(uint *data, uint *copy, const uint size, const uint index)
{
        if (size < 2)
                return 0L;

        const uint left_len  = size / 2;
        const uint right_len = size - left_len;

        Lint inv_count;
        inv_count  = do_merge_sort2(data,            copy, left_len,  index);
        inv_count += do_merge_sort2(&data[left_len], copy, right_len, index + left_len);

        inv_count += merge2(copy, data, &data[left_len], left_len, right_len, index);

        for (uint i = 0; i < size; ++i)
                data[i] = copy[i + index];

        return inv_count;
}


static Lint
merge2(uint *copy,
      const uint *left,
      const uint *right,
      const uint left_len,
      const uint right_len,
      const uint index)
{
        uint lc = 0, rc = 0;
        uint i = index;
        Lint inv_count = 0;

        Lint mid = (Lint)index + (Lint)left_len;

        while (lc < left_len && rc < right_len) {
                if (left[lc] <= right[rc]) {
                        copy[i++] = left[lc++];
                } else {
                        copy[i++] = right[rc++];
                        inv_count += mid - ((Lint)lc + (Lint)index);
                }
        }

        while (lc < left_len) {
                copy[i++] = left[lc++];
        }
        while (rc < right_len) {
                copy[i++] = right[rc++];
        }

        return inv_count;
}



#if 0
static void do_merge_sort2(uint *data, uint *copy, uint left, uint right);
static void merge2(uint *copy, uint *data, uint left, uint mid, uint right);


Lint
__merge_sort(uint *data, uint size)
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
