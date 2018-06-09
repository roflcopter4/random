#include "preproc.h"
#include <ctype.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

static size_t slurp_file(char ** restrict slurp, const char * restrict filename);
static size_t slurp_stdin(char **slurp);
static char *parse_line(char * restrict cur_line, char * restrict buf);

int LEVEL;


int
main(int argc, char **argv)
{
        extern int optind;
        program_name = argv[0];
        bool is_tty  = handle_options(argc, argv);
        argc -= optind;
        argv += optind;
        if (argc == 0 && is_tty)
                errx(3, "No files specified.\n");

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

        buf = xmalloc(size + 2);
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
        char *path;
        struct stat st;
        size_t size;

        path = realpath(filename, NULL);
        safe_stat(path, &st);
        size = (size_t)st.st_size;;
        fp   = fopen(path, "r");

        if (!fp)
                err(1, "Failed to open file '%s'", filename);
        if (!S_ISREG(st.st_mode))
                errx(1, "Invalid filetype '%s'\n", filename);

        *slurp = xmalloc(size + 1);
        fread(*slurp, 1, size, fp);
        
        (*slurp)[size] = '\0';
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
                        *slurp = xrealloc(*slurp, (size <<= 1));

                (*slurp)[it++] = ch;
        }

        (*slurp)[it++] = '\0';
        *slurp = xrealloc(*slurp, it);

        return it;
}


static char *
parse_line(char * restrict cur_line, char * restrict buf)
{
        char *orig  = cur_line;
        char *tmp   = buf;
        int loc_lev = LEVEL;
        bool iscpp  = false;

        if (*cur_line == '\0')
                return cur_line;
        if (*cur_line == '\n') {
                putchar('\n');
                return (cur_line + 1);
        }

        if ((*tmp++ = *cur_line++) == '#') {
                iscpp = true;
redo:
                if (loc_lev > 0)
                        for (int i = 0; i < (loc_lev * WIDTH); ++i)
                                *tmp++ = ' ';

                while (isblank(*cur_line))
                        ++cur_line;
        } else if (ALL) {
                --cur_line; /* oopsie.. */
                --tmp;
                if (loc_lev > 0)
                        for (int i = 0; i < (loc_lev * WIDTH) + 1; ++i)
                                *tmp++ = ' ';
        }

        while ((*tmp++ = *cur_line++) != '\n' && *cur_line)
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
                        cur_line  = orig + 1;
                        tmp   = buf + 1;
                        goto redo;
                }
        }

        puts(buf);
        return cur_line;
}
