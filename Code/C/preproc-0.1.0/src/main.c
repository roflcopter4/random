#include "preproc.h"
#include <ctype.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

static size_t slurp_file(char ** restrict slurp, const char * restrict filename);
static size_t slurp_stdin(char **slurp);
static char *parse_line(char * restrict line, char * restrict buf);

int LEVEL;


int
main(int argc, char **argv)
{
        extern int optind;
        program_name = argv[0];
        bool is_tty = handle_options(argc, argv);
        argc -= optind;
        argv += optind;
        if (argc == 0 && is_tty)
                xerr(3, "No files specified.\n");

        size_t size;
        char *slurp, *orig, *buf;
        int len = (argc > 0) ? strlen(argv[0]) : 0;

        /* If the filename ends in ".h" or the -H option was given then
         * ignore one level of indentation. */
        if (!NO_SKIP && ((len > 2 && argv[0][len-2] == '.'
                                  && argv[0][len-1] == 'h')
                         || IS_HEADER))
                --LEVEL;

        if ((!is_tty && argc == 0) || streq(argv[0], "-"))
                size = slurp_stdin(&slurp);
        else
                size = slurp_file(&slurp, argv[0]);

        buf = malloc(size + 1);
        orig = slurp;

        while (*slurp != '\0')
                slurp = parse_line(slurp, buf);

        free(orig);
        free(buf);
        return 0;
}


static size_t
slurp_file(char ** restrict slurp, const char * restrict filename)
{
        FILE *fp;
        char *path, *buf;
        struct stat st;
        size_t size;
        int ch;

        path = realpath(filename, NULL);
        safe_stat(path, &st);
        size = (size_t)st.st_size;;
        fp   = fopen(path, "r");

        if (!fp)
                xperror("Failed to open file '%s'", filename);
        if (!S_ISREG(st.st_mode))
                xerr(1, "Invalid filetype '%s'\n", filename);

        *slurp = buf = xmalloc(++size);

        while ((ch = fgetc(fp)) != EOF)
                *buf++ = (char)ch;
        
        *buf = '\0';
        fclose(fp);
        free(path);

        return size;
}



static size_t
slurp_stdin(char **slurp)
{
        int ch;
        size_t it, size;

        *slurp = xmalloc(STARTSIZE);
        size  = STARTSIZE;
        it = 0;

        while ((ch = getchar()) != EOF) {
                if (ch == '\r')
                        continue;
                if (it >= (size - 1))
                        *slurp = xrealloc(*slurp, (size += INCSIZE));

                (*slurp)[it++] = ch;
        }

        (*slurp)[it++] = '\0';
        *slurp = xrealloc(*slurp, it);

        return it;
}


static char *
parse_line(char * restrict line, char * restrict buf)
{
        char *orig = line;
        char *tmp = buf;
        int loc_lev = LEVEL;
        bool iscpp = false;

        if (*line == '\0')
                return line;
        if (*line == '\n') {
                putchar('\n');
                return (line + 1);
        }

        if ((*tmp++ = *line++) == '#') {
                iscpp = true;
redo:
                if (loc_lev > 0)
                        for (int i = 0; i < (loc_lev * WIDTH); ++i)
                                *tmp++ = ' ';

                while (isblank(*line))
                        ++line;
        } else if (ALL) {
                --line; /* oopsie.. */
                --tmp;
                if (loc_lev > 0)
                        for (int i = 0; i < (loc_lev * WIDTH) + 1; ++i)
                                *tmp++ = ' ';
        }

        while ((*tmp++ = *line++) != '\n' && *line != '\0')
                ;
        *(tmp - 1) = '\0';

        if (iscpp) {
                int match = 0;
                tmp = buf + 1;
                while (isblank(*tmp))
                        ++tmp;

                if (strncmp(tmp, "if", 2) == 0) {
                        ++LEVEL;
                } else if (strncmp(tmp, "end", 3) == 0) {
                        match = 2;
                        --LEVEL;
                        loc_lev = LEVEL;
                } else if (strncmp(tmp, "el", 2) == 0) {        
                        match = 3;
                        --loc_lev;
                }

                if (match == 2 || match == 3) {
                        iscpp = false;
                        line = orig + 1;
                        tmp = buf + 1;
                        goto redo;
                }
        }

        puts(buf);
        return line;
}
