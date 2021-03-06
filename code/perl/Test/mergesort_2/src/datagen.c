#include "mergesort.h"
#include <sodium.h>
#include <stdlib.h>
#include <string.h>

static int compare(const void *a, const void *b);


uint *
getlist(int64 size, int64 max)
{
        uint *list = xmalloc(size * sizeof(uint));
        for (int64 i = 0; i < size; ++i)
                list[i] = randombytes_uniform(max);

        return list;
}


uint *
cpylist(uint *list, int64 size)
{
        uint *list_copy = xmalloc(size * sizeof(uint));
        memcpy(list_copy, list, size * sizeof(uint));

        return list_copy;
}


void
lazysort(uint *list, int64 size)
{
        qsort(list, size, sizeof(uint), compare);
}


static int
compare(const void *a, const void *b)
{
        return (*(const uint *)a >= *(const uint *)b);
}

