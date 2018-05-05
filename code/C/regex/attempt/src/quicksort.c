#include "neotags.h"
#include <stdlib.h>

#define QS_SIZE_TYPE size_t

static void do_quick_sort(char **data, int64_t first, int64_t last);
static int64_t partition(char **data, int64_t first, int64_t last);


void
quick_sort(char **data, const int64_t size)
{
        do_quick_sort(data, 0, size - 1L);
}


static void
do_quick_sort(char **data, const int64_t first, const int64_t last)
{
        if (first < last) {
                int64_t pivot = partition(data, first, last);
                do_quick_sort(data, first, pivot - 1L);
                do_quick_sort(data, pivot + 1L, last);
        }
}


static int64_t
partition(char **data, const int64_t first, const int64_t last)
{
        char *pivot = data[first];
        int64_t left  = first + 1L;
        int64_t right = last;
        bool done = false;

        while (!done) {
                /* while (left <= right && data[left] <= pivot)
                        ++left;

                while (left <= right && data[right] >= pivot)
                        --right; */

                while (left <= right && strcasecmp(data[left], pivot) <= 0)
                        ++left;

                while (left <= right && strcasecmp(data[right], pivot) >= 0)
                        --right;

                if (left > right) {
                        done = true;
                } else {
                        char *temp = data[left];
                        data[left]  = data[right];
                        data[right] = temp;
                }
        }

        char *temp = data[first];
        data[first] = data[right];
        data[right] = temp;
        return right;
}
