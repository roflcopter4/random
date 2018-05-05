#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <stdarg.h>
#include <errno.h>
#include "myStrLib.h"

#define BUFFERSIZE 64

void initStr(int, ...);

int main(void)
{
    int c, iter;
    const char *HOME_str;
    char *GITDIR_str, *buf1, *buf2, *cur_line;
    struct dirent *ep;
    DIR *git_dir;
    FILE *list;

    initStr(4, &GITDIR_str, &buf1, &buf2, &cur_line);

    HOME_str = getenv("HOME");
    myStrCat(&GITDIR_str, (char*) HOME_str, "/personaldotfiles/");
    myStrCat(&buf1, GITDIR_str, "/src/listofiles.txt");

    list = fopen(buf1, "r");
    if (list == NULL)
        return 1;
    myStrFree(1, &buf1);

    while (1) {
        cur_line = malloc(BUFFERSIZE);
        iter = 0;
        if ( (c = fgetc(list)) == EOF ) {
            break;
        } else {
            do {
                cur_line[iter] = c;
                iter++;
            } while ( (c = fgetc(list)) != '\n' && c != EOF );
        }
        printf("Line: %s, size: %ld\n", cur_line, strlen(cur_line));

        git_dir = opendir(GITDIR_str);
        if (git_dir != NULL) {
            while ( (ep = readdir(git_dir)) ) {
                if ( strcmp(ep->d_name, cur_line) == 0 ) {
                    myStrCat(&buf1, GITDIR_str, ep->d_name);
                    myStrCat(&buf2, (char *) HOME_str,   "/TestDir/");
                    myStrApp(&buf2, ep->d_name);
                    puts(buf1);
                    puts(buf2);
                    remove(buf2);
                    printf("link success: '%d'\n", symlink(buf1, buf2));
                }
                myStrFree(2, &buf1, &buf2);
            }
        }
        myStrFree(1, &cur_line);
        (void) closedir(git_dir);
    }
    fclose(list);

    myStrFree(4, &GITDIR_str, &buf1, &buf2, &cur_line);

    return 0;
}


void initStr(int count, ...)
{
    va_list ap;
    int iter;
    char **ptr = NULL;

    va_start(ap, count);
    for (iter = 0; iter < count; iter++) {
        ptr = va_arg(ap, char **);
        if (ptr != NULL) {
            *ptr = NULL;
            ptr = NULL;
        }
    }
    va_end(ap);
}
