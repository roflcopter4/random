#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <stdarg.h>
#include <errno.h>
/*#include "ggets.h"*/
#include "inc/myStrLib.h"

#define BUFSIZE 256

void initStr(int, ...);
int GetAllLines(char ***, char *);
void splitStr(char *, char **, char **);

int main(int argc, char **argv)
{
    int iter, numlines;
    char *buf1, *buf2, *cur_line;
    char **StrArray;

    /* Sets all strings to NULL */
    initStr(3, &buf1, &buf2, cur_line);
    StrArray = NULL;

    /* Returns number of lines in file, and copies all lines to the string array */
    numlines = GetAllLines(&StrArray, argv[1]);
    /*myStrFree(1, &buf1);*/

    /* All lines in the file are filenames. Loop through the lines and check
     * whether they exist in the root of the git dir. If so, print the full
     * path of the file, the full path of the target link, then make the link. */
    for ( iter = 0; iter < numlines; iter++ ) {
        cur_line = StrArray[iter];
        printf("%s\n", cur_line);
        /*myStrFree(2, &buf1, &buf2);*/
        myStrFree(1, &cur_line);
    }

    /* Free everything again, just to be sure. There is no risk of a double free
     * as long as every previous free used the myStrFree function, which sets
     * freed pointers to null, and refuses to free null pointers. The array itself
     * must be freed the old fashioned way. */
    freeStrArray(&StrArray, numlines);
    myStrFree(3, &buf1, &buf2, &cur_line);

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
        buf = calloc(BUFSIZE, sizeof(char));
        iter = 0;
        if ( (ch = fgetc(file)) == EOF )
            break;
        else if ( ch == '\n' ) {
            x--;
            lines--;
        } else {
            do {
                buf[iter] = ch;
                iter++;
            } while ( (ch = fgetc(file)) != '\n' && ch != EOF );
            myStrCpy(&((*StrArray)[x]), buf);
        }
        myStrFree(1, &buf);
    }
    fclose(file);
    return lines;
}
