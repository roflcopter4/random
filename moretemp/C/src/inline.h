#ifndef INLINE_H
#define INLINE_H

#include "neotags.h"
#include <stdlib.h>


__attribute__((__warn_unused_result__)) __attribute__((__malloc__))
static inline void *
xmalloc(const size_t size)
{
        void *tmp = malloc(size);
        if (tmp == NULL)
                err(100, "Malloc call failed - attempted %zu bytes\n", size);
        return tmp;
}


__attribute__((__warn_unused_result__)) __attribute__((__malloc__))
static inline void *
xcalloc(const int num, const size_t size)
{
        void *tmp = calloc(num, size);
        if (tmp == NULL)
                err(125, "Calloc call failed - attempted %zu bytes\n", size);
        return tmp;
}

__attribute__((__warn_unused_result__))
void *
xrealloc(void *ptr, const size_t size)
{
        void *tmp = realloc(ptr, size);
        if (tmp == NULL)
                err(150, "Realloc call failed - attempted %zu bytes\n", size);
        return tmp;
}


#endif /* inline.h */
