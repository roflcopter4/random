#include "mergesort.h"
#include <ctype.h>
#include <errno.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/time.h>

static int get_twidth();
static int numdigits(uint number);

#define STARTSIZE (128 * sizeof(char))
#define INCSIZE   (16  * sizeof(char))

char *
my_fgetline(FILE *f)
{
        int ch, it, size;
        char *buf, *temp, *line;

        buf = temp = NULL;  /* Just in case... */
        buf  = xmalloc(STARTSIZE);
        size = STARTSIZE;

        it = 0;
        while ((ch = getc(f)) != EOF && ch != '\n') {
                if (ch == '\r')  /* Leave out the garbage. */
                        continue;

                /* If the buffer is full, extend it with realloc. */
                if (it >= (size - 1)) {
                        size += INCSIZE;
                        temp = xrealloc((void*)buf, size);
                        buf = temp;
                }
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


void
free_s_array(struct s_array *str_array)
{
        for (int i = 0; i < str_array->num; ++i)
                free(str_array->arr[i]);

        free(str_array->len);
        free(str_array->arr);
}


void *
xmalloc(size_t size)
{
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
                xeprintf(100, "Calloc call failed - attempted %zu bytes.\n", size);
        if (errno) {
                perror("Calloc call failed.\n");
                exit(1);
        }

        return tmp;
}


void *
xrealloc(void *ptr, size_t size)
{
        void *tmp = realloc(ptr, size);

        if (tmp == NULL)
                xeprintf(150, "Realloc call failed - attempted %zu bytes.\n", size);

        return tmp;
}


int64
xatoi(char *str)
{
        char *endptr;
        int64_t num = strtol(str, &endptr, 10);

        if (endptr == str)
                xeprintf(30, "Invalid integer '%s'.\n", str);

        return num;
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
pretty_print(uint *intlist, int64 size)
{
        int intlen = numdigits(g_maxsize);
        int dig_per_line = get_twidth() / (intlen + 2);

        setlocale(LC_ALL, "");
        int n = 0;
        for (uint i = 0; i < size; ++i) {
                printf("%'*u  ", intlen, intlist[i]);
                if (++n == dig_per_line) {
                        putchar('\n');
                        n = 0;
                }
        }
        putchar('\n');
}


static int
get_twidth()
{
        struct winsize size;
        if (ioctl(0, TIOCGWINSZ, (char *) &size) < 0)
                xeprintf(45, "TIOCGWINSZ error");
        return size.ws_col;
}


void
print_intlist(uint *intlist, int64 size)
{
        for (uint i = 0; i < size; ++i)
                if (i < size - 1)
                        printf("%'u  ", intlist[i]);
                else
                        printf("%'u\n", intlist[i]);
}


static int
numdigits(uint number)
{
        char tmp[BUFSIZ];
        setlocale(LC_ALL, "");
        snprintf(tmp, BUFSIZ, "%'u", number);
        return strlen(tmp);
}

