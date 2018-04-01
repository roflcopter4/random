#include "mergesort.h"
#include <stdlib.h>

static void do_quick_sort(uint *data, int64 first, int64 last);
static int64 partition(uint *data, int64 first, int64 last);


void
quick_sort(uint *data, uint size)
{
        do_quick_sort(data, 0, (int64)size - 1);
}


static void
do_quick_sort(uint *data, int64 first, int64 last)
{
        if (first < last) {
                int64 pivot = partition(data, first, last);
                do_quick_sort(data, first, pivot - 1);
                do_quick_sort(data, pivot + 1, last);
        }
}


static int64
partition(uint *data, int64 first, int64 last)
{
        uint pivot_val = data[first];
        int64 left  = first + 1;
        int64 right = last;
        bool done = false;

        while (!done) {
                while (left <= right && data[left] <= pivot_val)
                        ++left;

                while (left <= right && data[right] >= pivot_val)
                        --right;

                if (left > right) {
                        done = true;
                } else {
                        uint temp   = data[left];
                        data[left]  = data[right];
                        data[right] = temp;
                }
        }

        uint temp   = data[first];
        data[first] = data[right];
        data[right] = temp;
        return right;
}
