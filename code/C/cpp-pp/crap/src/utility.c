#include "preproc.h"
#include <stdlib.h>
#if 0
#include <errno.h>
#include <stdarg.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#endif

/* static int get_twidth();
static int numdigits(uint number); */

#if 0
#define STARTSIZE (128 * sizeof(char))
#define INCSIZE   (16  * sizeof(char))

int
my_fgetline(char **ptr, FILE *fp)
/* my_getline(char **ptr) */
{
        int ch, it, size;
        char *buf, *temp;

        buf = temp = NULL;  /* Just in case... */
        buf  = xmalloc(STARTSIZE);
        size = STARTSIZE;

        it = 0;
        while ((ch = fgetc(fp)) != EOF && ch != '\n') {
                if (ch == '\r')  /* Leave out the garbage. */
                        continue;

                /* If the buffer is full, extend it with realloc. */
                if (it >= (size - 1)) {
                        size += INCSIZE;
                        temp = xrealloc((void*)buf, size);
                        buf = temp;
                }

                buf[it++] = ch;
        }
        buf[it++] = '\0';

        /* Attempt to reduce the size of the string to the bare minimum. If
         * that fails then just give up. */
        temp = xrealloc(buf, it);
        *ptr = temp;

        return it;
}


int
countlines(char *filename)
{
        int ch, numlines;

        errno = 0;
        FILE *fp = fopen(filename, "r");
        if (errno) {
                char tmp[BUFSIZ];
                sprintf(tmp, "Error opening file '%s'", filename);
                perror(tmp);
                errno = 0;
                exit(127);
        }

        numlines = 0;
        while ((ch = getc(fp)) != EOF)
                if (ch == '\n')
                        ++numlines;

        fclose(fp);
        return numlines;
}


void *
xmalloc(size_t size)
{
        errno = 0;
        void *tmp = malloc(size);

        if (tmp == NULL)
                xeprintf(100, "Malloc call failed - attempted %zu bytes.\n", size);

        return tmp;
}


void *
xcalloc(int num, size_t size)
{
        errno = 0;
        void *tmp = calloc(num, size);

        if (tmp == NULL)
                xeprintf(125, "Calloc call failed - attempted %zu bytes.\n", size);

        return tmp;
}


void *
xrealloc(void *ptr, size_t size)
{
        errno = 0;
        void *tmp = realloc(ptr, size);

        if (tmp == NULL)
                xeprintf(150, "Realloc call failed - attempted %zu bytes.\n", size);

        return tmp;
}


int
xasprintf(char **restrict ptr, const char *restrict fmt, ...)
{
        va_list ap;
        va_start(ap, fmt);
        int size;

        if ((size = vasprintf(ptr, fmt, ap)) < 0)
                xeprintf(2, "Error allocating memory for asprintf.\n");

        va_end(ap);
        return size;
}
#endif


int64_t
__xatoi(char *str, bool strict)
{
        char *endptr;
        long int val = strtol(str, &endptr, 10);

        if ((endptr == str) || (strict && *endptr != '\0'))
                xeprintf(30, "Invalid integer '%s'.\n", str);

        return (int)val;
}


#if 0
#define safe_stat(PATH, ST)                                         \
     do {                                                           \
             if ((stat((PATH), (ST)) != 0))                         \
                     xperror("Failed to stat file '%s", (PATH));    \
     } while (0)

bool
file_is_reg(const char *path)
{
        struct stat st;
        safe_stat(path, &st);
        return S_ISREG(st.st_mode);
}


size_t
get_file_size(const char *path)
{
        struct stat st;
        safe_stat(path, &st);
        return (size_t)st.st_size;
}


FILE *
safe_fopen(const char * const restrict fname,
           const char * const restrict mode)
{
        FILE *fp = fopen(fname, mode);
        if (!fp)
                xperror("Failed to open file '%s'", fname);
        if (!file_is_reg(fname))
                xeprintf(1, "Invalid filetype '%s'\n", fname);

        return fp;
}
#endif
