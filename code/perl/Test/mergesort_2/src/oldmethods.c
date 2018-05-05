#include "mergesort.h"
#include <stdlib.h>


void
bubble_sort(uint32_t *data, int64 size)
{
        uint32_t tmp;
        uint32_t numops = 0;
        uint32_t last = size - 1;
        bool exchange = true;

        while (exchange && last >= 0) {
                exchange = false;
                for (uint32_t i = 0; i < last; ++i) {
                        if (data[i] > data[i+1]) {
                                ++numops;
                                tmp       = data[i];
                                data[i]   = data[i+1];
                                data[i+1] = tmp;
                                exchange  = true;
                        }
                }
                --last;
        }

        printf("Numops: %u\n", numops);
}


void
selection_sort(uint32_t *data, int64 size)
{
        for (uint32_t index = 0; index < size; ++index) {
                uint32_t smallest = index;
                for (uint32_t i = index; i < size; ++i)
                        if (data[i] < data[smallest])
                                smallest = i;

                uint32_t tmp   = data[index];
                data[index]    = data[smallest];
                data[smallest] = tmp;
        }
}


void
recursive_ss(uint32_t *data, int64 size)
{
        uint32_t smallest = 0;
        for (uint32_t i = 0; i < size; ++i)
                if (data[i] < data[smallest])
                        smallest = i;

        uint32_t tmp   = data[0];
        data[0]        = data[smallest];
        data[smallest] = tmp;

        if (size > 2)
                recursive_ss(&data[1], size - 1);
}
