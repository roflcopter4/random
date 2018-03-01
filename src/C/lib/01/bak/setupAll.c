#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <stdarg.h>
/*#include <unistd.h>*/
/*@-skipposixheaders@*/
#include <sys/types.h>
/*@=skipposixheaders@*/
#include <errno.h>
/*#include "ggets.h"*/
#include "myStrLib.h"

#define BUFFERSIZE 64

void initStr(int, ...);
int GetAllLines(char ***, char *);

int main(void/*int argc, char **argv*/)
{
    int iter, numlines;
    const char *HOME_str;
    char *GITDIR_str, *buf1, *buf2, *cur_line;
    char **StrArray;
    struct dirent *ep;
    DIR *git_dir;

    /* Sets all strings to NULL */
    initStr(4, &GITDIR_str, &buf1, &buf2, &cur_line);
    StrArray = NULL;

    /* Get home dir and setup dir strings */
    HOME_str = getenv("HOME");
    myStrCat(&GITDIR_str, (char*) HOME_str, "/personaldotfiles/");
    myStrCat(&buf1, GITDIR_str, "/src/listofiles.txt");

    /* Returns number of lines in file, and copies all lines to the string array */
    numlines = GetAllLines(&StrArray, buf1);
    myStrFree(1, &buf1);

    /* All lines in the file are filenames. Loop through the lines and check
     * whether they exist in the root of the git dir. If so, print the full
     * path of the file, the full path of the target link, then make the link. */
    for ( iter = 0; iter < numlines; iter++ ) {
        cur_line = StrArray[iter];
        git_dir = opendir(GITDIR_str);
        if (git_dir != NULL) {
            /* Loop through files in the directory listing. */
            while ( (ep = readdir(git_dir)) ) {
                if ( strcmp(ep->d_name, cur_line) == 0 ) {
                    myStrCat(&buf1, GITDIR_str, ep->d_name);
                    myStrCat(&buf2, (char *) HOME_str,   "/TestDir/");
                    myStrApp(&buf2, ep->d_name);
                    puts(buf1);
                    puts(buf2);
                    remove(buf2);
                    symlink(buf1, buf2);
                    perror("");
                }
                myStrFree(2, &buf1, &buf2);
            }
        }
        /* Might as well free each string in the array as we reach it. */
        myStrFree(1, &cur_line);
        (void) closedir(git_dir);
    }

    /* Free everything again, just to be sure. There is no risk of a double free
     * as long as every previous free used the myStrFree function, which sets
     * freed pointers to null, and refuses to free null pointers. The array itself
     * must be freed the old fashioned way. */
    free(StrArray);
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

int GetAllLines(char ***StrArray, char *filename)
{
    int ch, lines, x, iter;
    char *buf;
    FILE *file;

    lines = 0;
    file = fopen(filename, "r");
    if (file == NULL)
        exit(1);
    while (!feof(file)) {
        ch = fgetc(file);
        if (ch == '\n') {
            lines++;
        }
    }
    fclose(file);

    file = fopen(filename, "r");
    if (file == NULL)
        exit(1);

    getStrArray(StrArray, lines);

    for (x = 0; x < lines; x++) {
        buf = calloc(BUFFERSIZE, sizeof(char));
        iter = 0;
        if ( (ch = fgetc(file)) == EOF )
            break;
        else {
            do {
                buf[iter] = ch;
                iter++;
            } while ( (ch = fgetc(file)) != '\n' && ch != EOF );
        }
        myStrCpy(&((*StrArray)[x]), buf);
        myStrFree(1, &buf);
    }
    fclose(file);
    return lines;
}
