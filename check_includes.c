#include "neotags.h"
#include <ctype.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#define WINPATH "C:\\Program Files (x86)\\Windows Kits\\10\\Include\\10.0.17134.0\\ucrt"

static void handle_preproc(const char *pos);
static void find_header(const char *header);


void
check_includes(struct linked_list *ll, const char *const vim_buf)
{
        char *pos = (char *)vim_buf;
        while (pos != NULL) {
                if (*pos == '#')
                        handle_preproc(++pos);
                if ((pos = strchr(pos, '\n')) != NULL)
                        ++pos;
        }
}


static void
handle_preproc(const char *pos)
{
        while (isblank(*pos))
                ++pos;
        if (*pos != 'i' || strncmp(pos, "include", 7) != 0)
                return;
        while (*pos != '<' && *pos != '\0')
                ++pos;
        if (*pos == '\0')
                return;
        ++pos;

        char header[BUFSIZ];
        int it = 0;
        while (*pos != '>' && it < BUFSIZ)
                header[it++] = *pos++;
        header[it] = '\0';

        find_header(header);
}


static void
find_header(const char *header)
{
        warnx("Looking for header: \"%s\"", header);

        struct dirent *dir;
        DIR *dp = opendir(WINPATH);
        if (!dp)
                err(1, "Failed to open dir '%s'", WINPATH);

        while ((dir = readdir(dp)))
                eprintf("entry is %s\n", dir->d_name);
}
