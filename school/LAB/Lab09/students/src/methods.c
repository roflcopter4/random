#include "sort.h"
#include <stdlib.h>


void
bubble_sort(uint32_t *data, int size)
{
        uint32_t tmp;
        /*unsigned int numops = 0;*/
        int last = size - 1;
        bool exchange = true;

        while (exchange && last >= 0) {
                exchange = false;
                for (int i = 0; i < last; ++i) {
                        /*++numops;*/
                        if (data[i] > data[i+1]) {
                                tmp       = data[i];
                                data[i]   = data[i+1];
                                data[i+1] = tmp;
                                exchange  = true;
                        }
                }
                --last;
        }

        /*printf("Number of steps taken: %u\n", numops);*/
}


void
selection_sort(uint32_t *data, int size)
{
        for (int index = 0; index < size; ++index) {
                int smallest = index;
                for (int i = index; i < size; ++i)
                        if (data[i] < data[smallest])
                                smallest = i;

                uint32_t tmp   = data[index];
                data[index]    = data[smallest];
                data[smallest] = tmp;
        }
}


void
recursive_ss(uint32_t *data, int size)
{
        int smallest = 0;
        for (int i = 0; i < size; ++i)
                if (data[i] < data[smallest])
                        smallest = i;

        uint32_t tmp   = data[0];
        data[0]        = data[smallest];
        data[smallest] = tmp;

        if (size > 2)
                recursive_ss(&data[1], size - 1);
}
