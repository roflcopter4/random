#include "tree.h"
#include <stdlib.h>

static void do_quick_sort(QS_TYPE *data, int64_t first, int64_t last);
static int64_t partition(QS_TYPE *data, int64_t first, int64_t last);


void
quick_sort(QS_TYPE *data, const QS_SIZE_TYPE size)
{
        do_quick_sort(data, 0, (int64_t)size - 1);
}


static void
do_quick_sort(QS_TYPE *data, const int64_t first, const int64_t last)
{
        if (first < last) {
                int64_t pivot = partition(data, first, last);
                do_quick_sort(data, first, pivot - 1);
                do_quick_sort(data, pivot + 1, last);
        }
}


static int64_t
partition(QS_TYPE *data, const int64_t first, const int64_t last)
{
        const QS_TYPE pivot_val = data[first];
        int64_t left  = first + 1;
        int64_t right = last;
        bool done = false;

        while (!done) {
                while (left <= right && data[left] <= pivot_val)
                        ++left;

                while (left <= right && data[right] >= pivot_val)
                        --right;

                if (left > right) {
                        done = true;
                } else {
                        QS_TYPE temp = data[left];
                        data[left]  = data[right];
                        data[right] = temp;
                }
        }

        QS_TYPE temp = data[first];
        data[first] = data[right];
        data[right] = temp;
        return right;
}
