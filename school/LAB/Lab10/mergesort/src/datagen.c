#include "mergesort.h"
#include <sodium.h>
#include <stdlib.h>
#include <string.h>

#ifdef SPACE_INTS
static int numdigits(int number);
#endif
static int compare(const void *a, const void *b);


void
print_intlist(uint32_t *intlist, int size)
{
#ifdef SPACE_INTS
        int intlen = numdigits(size);
        for (int i = 0; i < size; ++i)
                printf("%*u, ", intlen, intlist[i]);
#else
        for (int i = 0; i < size; ++i)
                printf("%u, ", intlist[i]);
#endif

        puts("");
}


uint32_t *
getlist()
{
        uint32_t *list = xmalloc(numitems * sizeof(uint32_t));
        for (uint32_t i = 0; i < numitems; ++i)
                list[i] = randombytes_uniform(maxsize);

        return list;
}


uint32_t *
cpylist(uint32_t *list)
{
        uint32_t *list_copy = xmalloc(numitems * sizeof(uint32_t));
        memcpy(list_copy, list, numitems * sizeof(uint32_t));

        return list_copy;
}


void
lazysort(uint32_t *list, int size)
{
        qsort(list, size, sizeof(uint32_t), compare);
}


static int
compare(const void *a, const void *b)
{
        return (*(const uint32_t *)a > *(const uint32_t *)b);
}


#ifdef SPACE_INTS
static int
numdigits(int number)
{
        char tmp[BUFSIZ];
        snprintf(tmp, BUFSIZ, "%d", number);
        return strlen(tmp);
}
#endif
