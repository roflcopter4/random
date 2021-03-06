#include "students.h"
#include <bsd/bsd.h>
#include <ctype.h>
#include <errno.h>
#include <stdarg.h>
#include <stdlib.h>
#include <sys/time.h>

#define STARTSIZE (128 * sizeof(char))
#define INCSIZE   (16  * sizeof(char))

char *
shitty_fgetline(FILE *f)
{
        int ch, it, size;
        char *buf, *temp, *line;

        buf = temp = NULL;  /* Just in case... */
        buf  = die_malloc(STARTSIZE);
        size = STARTSIZE;

        it = 0;
        while ((ch = getc(f)) != EOF && ch != '\n') {
                /* Leave out the garbage. */
                if (ch == '\r')
                        continue;

                /* If the buffer is full, extend it with realloc. */
                if (it >= (size - 1)) {
                        size += INCSIZE;
                        temp = die_realloc((void*)buf, size);
                        buf = temp;
                }
                buf[it++] = toupper(ch);  /* We need uppercase strings. */
        }
        buf[it] = '\0';

        /* Attempt to reduce the size of the string to the bare minimum. If
         * that fails then just give up. */
        temp = die_realloc(buf, it + 1);
        line = temp;

        return line;
}


int
countlines(char *filename)
{
        int ch, numlines;
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


struct s_array
parsefile(int argc, char **argv, int optind)
{
        char filename[BUFSIZ];
        if (argc > optind)
                strlcpy(filename, argv[optind], BUFSIZ);
        else
                strlcpy(filename, FILENAME, BUFSIZ);

        int numlines     = countlines(filename);
        int *numarr      = die_malloc(numlines * sizeof(int));
        char **str_array = die_malloc(numlines * sizeof(char *));

        /* We already opened this once, so this SHOULD work... */
        FILE *fp = fopen(filename, "r");
        for (int i = 0; i < numlines; ++i) {
                str_array[i] = shitty_fgetline(fp);
                numarr[i]    = strlen(str_array[i]);
        }
        fclose(fp);

        struct s_array data = {str_array, numarr, numlines};
        return data;
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
        for (int i = 0; i < str_array->num; ++i) {
                free(str_array->arr[i]);
        }
        free(str_array->len);
        free(str_array->arr);
}


void *
die_malloc(size_t size)
{
        void *tmp = malloc(size);
        if (tmp == NULL) {
                eprintf("Malloc call failed - attempted %lu bytes.\n", size);
                exit(100);
        }

        return tmp;
}


void *
die_realloc(void *ptr, size_t size)
{
        void *tmp = realloc(ptr, size);
        if (tmp == NULL) {
                eprintf("Realloc call failed - attempted %lu bytes.\n", size);
                exit(150);
        }

        return tmp;
}


void
print_array(char **array, int len, char nl)
{
        for (int i = 0; i < len; ++i) {
                if (nl == 'y') {
                        printf("%s\n", array[i]);
                } else {
                        if (i < (len - 1))
                                printf("%s, ", array[i]);
                        else
                                printf("%s\n", array[i]);
                }
        }
}


int
xatoi(char *str)
{
        char *endptr;
        int num = strtol(str, &endptr, 10);

        if (endptr == str) {
                eprintf("Invalid integer '%s'.\n", str);
                exit(2);
        }

        return num;
}
