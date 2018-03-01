#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "myStrLib.h"

#define CHARSIZE sizeof(char)
#define A_INDEX 65
#define ALPHABET_LEN 26
#define BACKUP_FILE "secret.txt"
#define modulo(x,n) ((x % n + n) % n)
/* Why the hell doesn't c have an actual modulo operator? */

typedef struct {
    char *cypher;
    char *cleartext;
    int key, has_key;
} Crypt;

Crypt parse_line(char *line);
void decode(Crypt *cr);


int main(int argc, char **argv)
{
    int numLines, iter, found_file;
    char **StrArray = NULL;

    found_file = 0;
    for (iter = 1; iter < argc && !found_file; iter++) {
        if ((numLines = GetAllLines(&StrArray, argv[iter], 0)) != -1)
            found_file = 1;
    }
    if (!found_file) {
        if (argc == 1)
            puts("No filenames supplied, reverting to backup filename.");
        else
            puts("All given files failed to open, reverting to backup filename.");

        if ((numLines = GetAllLines(&StrArray, BACKUP_FILE, 0)) == -1) {
            printf("The backup file, '%s' failed to open. \n", BACKUP_FILE);
            puts("Can't be bothered to add interactivity. Bailing out.");
            exit(1);
        }
    }

    Crypt crypts[numLines];
    Crypt *cr;

    for (iter = 0; iter < numLines; iter++) {
        cr = &crypts[iter];
        *cr = parse_line(StrArray[iter]);
        decode(cr);
    }

    /* Obviously it makes no sense to print and free everything in a separate
     * loop, but under the pretext that I might want to do something with the
     * data in the meantime I'll do that anyway. */
    for (iter = 0; iter < numLines; iter++) {
        cr = &crypts[iter];
        if (cr->has_key)
            printf("%s\n", cr->cleartext);
        else
            puts("No key supplied!");

        safeFree(cr->cypher);
        safeFree(cr->cleartext);
    }

    freeStrArray(&StrArray, numLines);
    return 0;
}


Crypt parse_line(char *line)
{
    char *str1, *str2;
    char ch;
    unsigned int iter1, iter2;
    size_t len;
    Crypt cr;
    cr.cypher = cr.cleartext = NULL;

    len = strlen(line);
    saferCalloc(str1, len+1, CHARSIZE);
    iter1 = 0;
    while ((ch = line[iter1]) != ' ' && (iter1 < len)) {
        str1[iter1] = ch;
        iter1++;
    }

    if (ch == ' ' && iter1 < len) {
        iter1++;
        iter2 = 0;
        saferCalloc(str2, (len - iter1) + 1, CHARSIZE);
        while (iter1 < len) {
            str2[iter2] = line[iter1];
            iter1++;
            iter2++;
        }
        cr.key = atoi(str2);
        cr.has_key = 1;
        safeFree(str2);
    } else {
        cr.key = 0;
        cr.has_key = 0;
    }
    myStrCpy(&cr.cypher, str1);

    safeFree(str1);
    return cr;
}


/* Originally this function also printed the data, but I thought that it
 * doesn't really make sense for it to do that, it should just decode. */
void decode(Crypt *cr)
{
    int ch;
    unsigned int x;
    size_t len;
    len = strlen(cr->cypher);

    if (len > 0) {
        saferCalloc(cr->cleartext, len+1, CHARSIZE);
        if (cr->has_key) {
            for (x = 0; x < len; x++) {
                ch = cr->cypher[x];
                ch = modulo((ch - A_INDEX + cr->key), ALPHABET_LEN) + A_INDEX;
                cr->cleartext[x] = ch;
            }
        } 
    }
}
