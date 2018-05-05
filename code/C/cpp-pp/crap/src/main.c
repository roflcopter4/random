#include "preproc.h"
#include <ctype.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

static char *slurp_file(char *filename);
static char *slurp_stdin(void);
static char *parse_line(char *line);

#define safe_stat(PATH, ST)                                      \
     do {                                                        \
             if ((stat((PATH), (ST)) != 0))                      \
                     xperror("Failed to stat file '%s", (PATH)); \
     } while (0)

int level;


int
main(int argc, char **argv)
{
        extern int optind;
        program_name = argv[0];
        handle_options(argc, argv);
        argc -= optind;
        argv += optind;
        if (argc < 1)
                xeprintf(3, "No files specified.\n");

        int len = strlen(argv[0]);
        /*
         * If the filename ends in ".h" or the -H option was given, ignore one
         * level of indentation.
         */
        if (!NO_SKIP && ((len > 2 && argv[0][len - 2] == '.' &&
                                     argv[0][len - 1] == 'h')
                         || IS_HEADER))
                --level;

        char *slurp;

        if (streq(argv[0], "-"))
                slurp = slurp_stdin();
        else
                slurp = slurp_file(argv[0]);

        char *orig  = slurp;

        while (*slurp != '\0')
                slurp = parse_line(slurp);

        free(orig);
        return 0;
}


static char *
slurp_file(char *filename)
{
        FILE *fp;
        char *path, *slurp, *orig;
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
                xeprintf(1, "Invalid filetype '%s'\n", filename);

        slurp = orig = malloc(++size * sizeof(*slurp));

        while ((ch = fgetc(fp)) != EOF)
                *slurp++ = (char)ch;
        
        *slurp = '\0';
        fclose(fp);
        free(path);

        return orig;
}



static char *
slurp_stdin(void)
{
        int ch, it, size;
        char *slurp, *tmp;

        slurp = malloc(STARTSIZE);
        size  = STARTSIZE;
        it = 0;

        while ((ch = getchar()) != EOF) {
                if (ch == '\r')  /* Leave out the garbage. */
                        continue;

                /* If the buffer is full, extend it with realloc. */
                if (it >= (size - 1)) {
                        size += INCSIZE;
                        tmp = realloc((void*)slurp, size);
                        slurp = tmp;
                }

                slurp[it++] = ch;
        }

        slurp[it++] = '\0';

        /* Attempt to reduce the size of the string to the bare minimum. If
         * that fails then just give up. */
        tmp = realloc(slurp, it);
        slurp = tmp;

        return slurp;
}


#define NOP

static char *
parse_line(char *line)
{
        char *orig = line;
        char buf[BUFSIZ];
        char *tmp = buf;
        int loc_lev = level;
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
                --line;  /* oopsie.. */
                --tmp;
                if (loc_lev > 0)
                        for (int i = 0; i < (loc_lev * WIDTH) + 1; ++i)
                                *tmp++ = ' ';
        }

        while ((*tmp++ = *line++) != '\n')
                NOP;
        *(tmp - 1) = '\0';

        if (iscpp) {
                int match = 0;
                tmp = buf + 1;
                while (isblank(*tmp))
                        ++tmp;

                if (strncmp(tmp, "if", 2) == 0) {
                        ++level;
                } else if (strncmp(tmp, "end", 3) == 0) {
                        match = 2;
                        --level;
                        loc_lev = level;
                } else if (strncmp(tmp, "else", 4) == 0) {        
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
