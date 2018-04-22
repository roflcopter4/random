#include "preproc.h"
#include <stdlib.h>

void * xmalloc(size_t size)
{
        void *tmp = malloc(size);
        if (tmp == NULL)
                xerr(100, "Malloc call failed - attempted %zu bytes.\n", size);
        return tmp;
}

void * xcalloc(int num, size_t size)
{
        void *tmp = calloc(num, size);
        if (tmp == NULL)
                xerr(105, "Calloc call failed - attempted %zu bytes.\n", size);
        return tmp;
}

void * xrealloc(void *ptr, size_t size)
{
        void *tmp = realloc(ptr, size);
        if (tmp == NULL)
                xerr(110, "Realloc call failed - attempted %zu bytes.\n", size);
        return tmp;
}

int64_t __xatoi(char *str, bool strict)
{
        char *endptr;
        long int val = strtol(str, &endptr, 10);
        if ((endptr == str) || (strict && *endptr != '\0'))
                xerr(30, "Invalid integer '%s'.\n", str);
        return (int)val;
}
