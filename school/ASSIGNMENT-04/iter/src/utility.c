#include "tree.h"
#include <errno.h>
#include <stdlib.h>
#include <string.h>

void *
xmalloc(size_t size)
{
        errno = 0;
        void *tmp = malloc(size);

        if (tmp == NULL)
                xeprintf(100, "Malloc call failed - attempted %zu bytes.\n", size);
        if (errno) {
                perror("Malloc call failed.\n");
                exit(100);
        }

        return tmp;
}


void *
xcalloc(int num, size_t size)
{
        errno = 0;
        void *tmp = calloc(num, size);

        if (tmp == NULL)
                xeprintf(125, "Calloc call failed - attempted %zu bytes.\n", size);
        if (errno) {
                perror("Calloc call failed.\n");
                exit(125);
        }

        return tmp;
}


void *
xrealloc(void *ptr, size_t size)
{
        errno = 0;
        void *tmp = realloc(ptr, size);

        if (tmp == NULL)
                xeprintf(150, "Realloc call failed - attempted %zu bytes.\n", size);
        if (errno) {
                perror("Realloc call failed.\n");
                exit(150);
        }

        return tmp;
}


inline void
xfree(void *ptr)
{
        if (ptr != NULL)
                free(ptr);
}


int64_t
__xatoi(char *str, bool strict)
{
        char *endptr;
        long int val = strtol(str, &endptr, 10);

        if ((endptr == str) || (strict && *endptr != '\0'))
                xeprintf(30, "Invalid integer '%s'.\n", str);

        return (int)val;
}
