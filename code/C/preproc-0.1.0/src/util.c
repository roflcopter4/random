#include "preproc.h"
#include <errno.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

void *xmalloc(const size_t size)
{
        void *tmp = malloc(size);
        if (tmp == NULL)
                err(100, "Malloc call failed - attempted %zu bytes", size);
        return tmp;
}


void *xcalloc(const int num, const size_t size)
{
        void *tmp = calloc(num, size);
        if (tmp == NULL)
                err(101, "Calloc call failed - attempted %zu bytes", size);
        return tmp;
}


void *xrealloc(void *ptr, const size_t size)
{
        void *tmp = realloc(ptr, size);
        if (tmp == NULL)
                err(102, "Realloc call failed - attempted %zu bytes", size);
        return tmp;
}


#ifdef HAVE_REALLOCARRAY
void *xreallocarray(void *ptr, size_t num, size_t size)
{
        void *tmp = reallocarray(ptr, num, size);
        if (tmp == NULL)
                err(103, "Realloc call failed - attempted %zu bytes", size);
        return tmp;
}
#endif

int64_t __xatoi(char *str, bool strict)
{
        char *endptr;
        long int val = strtol(str, &endptr, 10);
        if ((endptr == str) || (strict && *endptr != '\0'))
                errx(30, "Invalid integer '%s'.\n", str);
        return (int)val;
}


#ifndef HAVE_ERR
void
_warn(bool print_err, const char *const __restrict fmt, ...)
{
        va_list ap;
        va_start(ap, fmt);
        char buf[BUFSIZ];

        if (print_err)
                snprintf(buf, BUFSIZ, "%s: %s: %s\n", program_name, fmt,
                         strerror(errno));
        else
                snprintf(buf, BUFSIZ, "%s: %s\n", program_name, fmt);

        vfprintf(stderr, buf, ap);

        va_end(ap);
}
#endif
