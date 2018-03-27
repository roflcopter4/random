#include "sort.h"
#include <stdlib.h>
#include <string.h>

static int compare(const void *a, const void *b);


uint32_t *getlist()
{
        uint32_t *list = die_malloc(numitems * sizeof(uint32_t));
        for (int i = 0; i < numitems; ++i)
                list[i] = randombytes_uniform(maxsize);

        return list;
}


uint32_t *cpylist(uint32_t *list)
{
        uint32_t *list_copy = die_malloc(numitems * sizeof(uint32_t));
        memcpy(list_copy, list, numitems * sizeof(uint32_t));

        return list_copy;
}


int numdigits(int number)
{
        char tmp[BUFSIZ];
        snprintf(tmp, BUFSIZ, "%d", number);
        return strlen(tmp);
}


void lazysort(uint32_t *list, int size)
{
        qsort(list, size, sizeof(uint32_t), compare);
}


static int compare(const void *a, const void *b)
{
        return (*(const uint32_t *)a > *(const uint32_t *)b);
}
