#include "fool.h"
#include <errno.h>
#include <limits.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/time.h>

#if 0
static int get_twidth();
static int numdigits(uint32_t number);
#endif

#define STARTSIZE 128
#define INCSIZE   32

char *
my_fgetline(FILE *fp, int *len)
{
        int ch;
        char *tmp;
        char *buf   = xmalloc(STARTSIZE);
        size_t size = STARTSIZE;
        size_t i    = 0;

        while ((ch = getc(fp)) != EOF && ch != '\n') {
                if (ch == '\r')
                        continue;

                /* Doing it this way makes it possible to detect an allocation
                 * error and return a partial buffer if desired. */
                if (i >= (size - 1)) {
                        tmp = xrealloc(buf, (size += INCSIZE));
                        buf = tmp;
                }

                buf[i++] = (char)ch;
        }
        buf[i++] = '\0';

        /* Attempt to reduce the size of the string to the bare minimum. Again,
         * it is possible to return a partial buffer if desired. */
        tmp = xrealloc(buf, i);
        buf = tmp;

        if (len != NULL)
                *len = (int)i;

        return buf;
}


int
countlines(const char *filename)
{
        int ch, numlines;

        errno = 0;
        FILE *fp = fopen(filename, "r");
        if (errno) {
                char tmp[BUFSIZ];
                snprintf(tmp, BUFSIZ, "Error opening file '%s'", filename);
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


#if 0
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
#endif


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
        void *tmp = calloc((size_t)num, size);

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


#if 0
void
xfree(void *ptr)
{
        if (ptr != NULL)
                free(ptr);
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
#endif


#if 0
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

#endif


struct str_list *
getlines(const char *filename)
{
        int numlines = countlines(filename);
        if (numlines <= 0)
                xeprintf(1, "Error: empty file.\n");
        struct str_list *lines = make_str_list((uint32_t)numlines);

        FILE *fp = fopen(filename, "r");
        for (int i = 0; i < numlines; ++i) {
                int len;
                char *buf = my_fgetline(fp, &len);
                lines->data[i]  = buf;
                lines->sizes[i] = (uint)len;
        }
        fclose(fp);

        return lines;
}


struct str_list *
make_str_list(uint32_t numstrings)
{
        struct str_list *list = malloc(sizeof *list);
        list->data  = malloc(numstrings * sizeof *list->data);
        list->sizes = malloc(numstrings * sizeof *list->sizes);
        list->num   = numstrings;

        return list;
}


void
destroy_str_list(struct str_list *list)
{
        free(list->sizes);
        for (uint32_t i = 0; i < list->num; ++i)
                free(list->data[i]);
        free(list->data);
        free(list);
}

