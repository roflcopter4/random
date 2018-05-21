#include "neotags.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#ifdef DOSISH
#   include "WIN/dirent.h"
#else
#   include <dirent.h>
#endif

/* #define WINPATH "C:\\Program Files (x86)\\Windows Kits\\10\\Include\\10.0.17134.0\\ucrt" */
#define WINPATH "C:\\Program Files (x86)\\Windows Kits\\10\\Include\\10.0.16299.0\\ucrt" 
/* #define WINPATH "D:\\cygwin64\\usr\\include" */
FILE* _log;
#define lprintf(...) fprintf(_log, __VA_ARGS__)

#define isdot(FNAME)                                 \
        ((FNAME)[0] == '.' && ((FNAME)[1] == '\0' || \
                               ((FNAME)[1] == '.' && (FNAME)[2] == '\0')))

static void handle_preproc(const char* pos);
static char *find_header(const char* header, const char* search_dir);

void check_includes(struct linked_list* ll, const char* const vim_buf)
{
        char* pos = (char*)vim_buf;
        _log = fopen("C:\\Users\\Mark Leason\\ntlog.log", "wb");
        while (pos != NULL) {
                if (*pos == '#')
                        handle_preproc(++pos);
                if ((pos = strchr(pos, '\n')) != NULL)
                        ++pos;
        }
        fclose(_log);
}

static void handle_preproc(const char* pos)
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

        char* path = find_header(header, WINPATH);
        if (path) {
                lprintf("Found path %s\n", path);
                free(path);
        } else {
                lprintf("Couldn't find header %s\n", header);
        }
}

#define DIR_BUFSIZ 2048
static char* find_header(const char* header, const char* search_dir)
{
        struct dirent* dir;
        char* ret = NULL;
        DIR* dp = opendir(search_dir);
        if (!dp)
                err(1, "Failed to open dir '%s'\n", WINPATH);

        char* subdir = strchr(header, '/');

        while ((dir = readdir(dp))) {
                if (isdot(dir->d_name))
                        continue;

                if (subdir) {
                        if (dir->d_type == DT_DIR && strncmp(dir->d_name,
                            header, subdir - header - 1) == 0)
                        {
                                char tmp[DIR_BUFSIZ];
                                snprintf(tmp, DIR_BUFSIZ, "%s%c%s", search_dir,
                                         PATHSEP, dir->d_name);
                                if ((ret = find_header(subdir + 1, tmp)))
                                        break;
                        }
                } else if (dir->d_type == DT_REG &&
                           streq(header, dir->d_name))
                {
                        size_t size = strlen(search_dir) + dir->d_namlen + 2;
                        ret = xmalloc(size);
                        sprintf(ret, "%s%c%s", search_dir, PATHSEP, dir->d_name);
                        break;
                }
        }

        closedir(dp);
        return ret;
}
