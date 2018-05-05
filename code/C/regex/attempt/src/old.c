#include "neotags.h"
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

#define REQUIRED_INPUT 4

static bool skip_tag(char **skip, const char *find);
static void get_skipped_tags(char **skip, char *arg);
static void sanity_check(int argc);

int
main(int argc, char **argv)
{
        sanity_check(argc);
        char *progname = *argv++;

        char *find;
        int64_t i;
        int64_t chars = xatoi(*argv++);
        char *data    = malloc((chars + 2L) * sizeof(*data));
        char **skip   = malloc((xatoi(*argv++) + 1L) * sizeof(*skip));

        get_skipped_tags(skip, *argv++);

        for (i = 0; i < chars;)
                data[i++] = getchar();
        data[i] = '\0';

        while ((find = *argv++) != NULL) {
                if (skip_tag(skip, find) || !strstr(data, find))
                        puts("False");
                else
                        puts("True");
        }

        argv = skip;
        while ((find = *argv++) != NULL)
                free(find);

        free(skip);
        free(data);
        return 0;
}


static void
get_skipped_tags(char **skip, char *arg)
{
        int ch, it = 0;
        char buf[BUFSIZ];

        while ((ch = *arg++) != '\0') {
                if (ch == ':') {
                        buf[it] = '\0';
                        *skip++ = strdup(buf);
                        it = 0;
                } else {
                        buf[it++] = (char)ch;
                }
        }

        *skip = NULL;
}


static bool
skip_tag(char **skip, const char *find)
{
        const char *buf;
        while ((buf = *skip++) != NULL)
                if (streq(buf, find)) 
                        return true;
        return false;
}


static void
sanity_check(int argc)
{
        if (isatty(0))
                xeprintf(2, "This program can't be run manually.\n");
        if (argc < REQUIRED_INPUT)
                exit(1);
}
