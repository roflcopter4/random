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

int main(void/*int argc, char **argv*/)
{
    int iter, numlines;
    const char *HOME_str;
    char *GITDIR_str, *buf1, *buf2;
    char **StrArray;
    DIR *git_dir;

    /* Sets all strings to NULL */
    initStr(3, &GITDIR_str, &buf1, &buf2);
    StrArray = NULL;

    /* Get home dir and setup dir strings */
    HOME_str = getenv("HOME");
    myStrCat(&GITDIR_str, (char*) HOME_str, "/personaldotfiles/");
    myStrCat(&buf1, GITDIR_str, "/src/listofiles1.txt");
    printf("Git dir is %s.\n", GITDIR_str);

    /* Returns number of lines in file, and copies all lines to the string array */
    numlines = GetAllLines(&StrArray, buf1);
    myStrFree(1, &buf1);


    /* All lines in the file are filenames. Loop through the lines and check
     * whether they exist in the root of the git dir. If so, print the full
     * path of the file, the full path of the target link, then make the link. */
    for ( iter = 0; iter < numlines; iter++ ) {
        char *cur_line = NULL;
        cur_line = StrArray[iter];
        git_dir = opendir(GITDIR_str);
        if (git_dir != NULL) {
            struct dirent *ep;
            /* Loop through files in the directory listing. */
            while ( (ep = readdir(git_dir)) ) {
                if ( strcmp(ep->d_name, cur_line) == 0 ) {
                    myStrCat(&buf1, GITDIR_str, ep->d_name);
                    /*myStrCat(&buf2, (char *) HOME_str,   "/TestDir/");*/
                    myStrCat(&buf2, (char *) HOME_str, "/");
                    myStrApp(&buf2, ep->d_name);
                    printf("Linking %s to \n\t%s\n", cur_line, buf2);
                    /*remove(buf2);*/
                    /*symlink(buf1, buf2);*/
                    if ( errno )
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
    StrArray = NULL;
    
    myStrCat(&buf1, GITDIR_str, "/src/listofiles2.txt");
    numlines = GetAllLines(&StrArray, buf1);
    myStrFree(1, &buf1);

    for ( iter = 0; iter < numlines; iter++ ) {
        char *source, *dest;
        char buf3[BUFSIZE];
        initStr(2, &source, &dest);
        source = StrArray[iter];
        dest = StrArray[++iter];
        /*printf("Source is %s, dest is %s.\n", source, dest);*/
        myStrCat(&buf1, GITDIR_str, source);
        myStrCat(&buf2, (char *) HOME_str, "/");
        myStrApp(&buf2, dest);
        sprintf(buf3, "mkdir -p \"$(dirname %s)\"", buf2);
        /*system(buf3);*/
        printf("Linking %s to \n\t%s\n", source, buf2);
        /*remove(buf2);*/
        if ( errno )
            perror("");
        /*symlink(buf1, buf2);*/
        myStrFree(2, &buf1, &buf2);
    }
    freeStrArray(&StrArray, numlines);

    myStrFree(3, &buf1, &buf2, &GITDIR_str);

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
