#include "arse.h"
#include <errno.h>
#include <limits.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/time.h>

static int get_twidth();
static int numdigits(uint32_t number);

#define STARTSIZE (128 * sizeof(char))
#define INCSIZE   (32  * sizeof(char))

char *
my_fgetline(FILE *fp)
{
        int ch, it, size;
        char *buf, *temp, *line;

        buf  = temp = line = NULL;  /* Just in case... */
        buf  = xmalloc(STARTSIZE);
        size = STARTSIZE;

        it = 0;
        while ((ch = getc(fp)) != EOF && ch != '\n') {
                if (ch == '\r')  /* Leave out the garbage. */
                        continue;

                /* If the buffer is full, extend it with realloc. */
                if (it >= (size - 1)) {
                        size += INCSIZE;
                        temp  = xrealloc((void *)buf, size);
                        buf   = temp;
                }

                buf[it++] = (char)ch;
        }
        buf[it] = '\0';

        /* Attempt to reduce the size of the string to the bare minimum. If
         * that fails then just give up. */
        temp = xrealloc(buf, it + 1);
        line = temp;

        return line;
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


/* I have no idea how this works. */
void
shuffle(char **array, size_t n)
{
        struct timeval tv;
        gettimeofday(&tv, NULL);
        int usec = tv.tv_usec;
        srand48(usec);

        if (n > 1) {
                size_t i;
                for (i = n - 1; i > 0; --i) {
                        size_t j = (unsigned int) (drand48()*(i+1));
                        char *t  = array[j];
                        array[j] = array[i];
                        array[i] = t;
                }
        }
}


#if 0
void
free_s_array(struct s_array *str_array)
{
        for (int i = 0; i < str_array->num; ++i)
                free(str_array->arr[i]);

        free(str_array->len);
        free(str_array->arr);
}
#endif


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


void
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


void
print_array(char **array, int len)
{
        for (int i = 0; i < len; ++i) {
                if (i < (len - 1))
                        printf("%s, ", array[i]);
                else
                        printf("%s\n", array[i]);
        }
}


void
pretty_print(uint32_t *intlist, uint32_t size)
{
        int intlen = numdigits(size);
        int dig_per_line = get_twidth() / (intlen + 2);

        setlocale(LC_NUMERIC, "");
        int n = 0;
        for (uint32_t i = 0; i < size; ++i) {
                printf("%'*u  ", intlen, intlist[i]);
                if (++n == dig_per_line) {
                        putchar('\n');
                        n = 0;
                }
        }
        putchar('\n');
}


void
print_intlist(uint32_t *intlist, uint32_t size)
{
        for (uint32_t i = 0; i < size; ++i)
                if (i < size - 1)
                        printf("%'u  ", intlist[i]);
                else
                        printf("%'u\n", intlist[i]);
}


static int
get_twidth()
{
        struct winsize size;
        if (ioctl(0, TIOCGWINSZ, (char *) &size) < 0)
                xeprintf(45, "TIOCGWINSZ error");
        return size.ws_col;
}


static int
numdigits(uint32_t number)
{
        char tmp[BUFSIZ];
        setlocale(LC_NUMERIC, "");
        snprintf(tmp, BUFSIZ, "%'u", number);
        return strlen(tmp);
}


