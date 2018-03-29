#include "mergesort.h"
#include <stdlib.h>
#include <string.h>

static int64_t do_merge_sort(uint *data, uint *copy, uint size, uint index);
static int64_t merge(uint *copy, const uint *left, const uint *right,
                     uint left_len, uint right_len, uint index);


int64_t
count_invertions(uint *data, uint size)
{
        uint *copy = xmalloc(size * sizeof(uint));
        int64_t inv_count = do_merge_sort(data, copy, size, 0);
        free(copy);
        return inv_count;
}


static int64_t
do_merge_sort(uint *data, uint *copy, const uint size, const uint index)
{
        if (size < 2)
                return 0L;

        const uint left_len  = size / 2;
        const uint right_len = size - left_len;

        int64_t inv_count;
        inv_count  = do_merge_sort(data,            copy, left_len,  index);
        inv_count += do_merge_sort(&data[left_len], copy, right_len, index + left_len);

        inv_count += merge(copy, data, &data[left_len], left_len, right_len, index);

        for (uint i = 0; i < size; ++i)
                data[i] = copy[i + index];

        return inv_count;
}


static int64_t
merge(uint *copy,
      const uint *left,
      const uint *right,
      const uint left_len,
      const uint right_len,
      const uint index)
{
        uint lc = 0, rc = 0;
        uint i   = index;
        uint mid = index + left_len;

        int64_t inv_count = 0;

        while (lc < left_len && rc < right_len) {
                if (left[lc] <= right[rc]) {
                        copy[i++]  = left[lc++];
                } else {
                        copy[i++]  = right[rc++];
                        inv_count += mid - (lc + index);
                }
        }

        while (lc < left_len)
                copy[i++] = left[lc++];
        while (rc < right_len)
                copy[i++] = right[rc++];

        return inv_count;
}

