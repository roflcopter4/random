#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "myStrLib.h"

#define CHARSIZE sizeof(char)
static void noMem();


void myStrCpy(char **strto, char *strfrom)
{
    size_t len;
    len = strlen(strfrom) + 1;

    /* If string not null, free the memory first. */
    if (*strto != NULL)
        free(*strto);

    /* Allocate just enough memory to hold the whole string. */
    *strto = calloc(len, CHARSIZE);
    /* Check whether calloc failed to allocate enough memory. */
    if (*strto == NULL)
        noMem();

    /* Copy the string to the new buffer. */
    memcpy(*strto, strfrom, len);
}


void myStrCat(char **strto, char *str1, char *str2)
{
    size_t len1, len2, lentot;
    len1 = strlen(str1);
    len2 = strlen(str2);
    lentot = len1 + len2 + 1;

    /* If string not null, free the memory first. */
    if (*strto != NULL)
        free(*strto);

    /* Allocate just enough memory to hold the whole string. */
    *strto = calloc(lentot, CHARSIZE);
    /* Check whether calloc failed to allocate enough memory. */
    if (*strto == NULL)
        noMem();

    /* Copy the first string to the new buffer. */
    memcpy(*strto, str1, len1);
    /* Copy the second string, starting where the first one ended. */
    memcpy( (*strto + (CHARSIZE * (len1))), str2, len2 );
}


void myStrApp(char **strOrig, char *strToAppend)
{
    char *buf = NULL;

    /* If the original string is empty, we can simply copy the other one to it. */
    if (*strOrig == NULL) {
        myStrCpy(strOrig, strToAppend);
    } else { /* Otherwise we make a copy of the original and then concatenate it with the other. */
        myStrCpy(&buf, *strOrig);
        myStrCat(strOrig, buf, strToAppend);
        free(buf);
    }
}


/* Returns an array of null string pointers of the designated size. The array
 * must initially be null or this routine will simply exit the program. */
void getStrArray(char ***StrArray, int NumStrings)
{
    int iter;

    if (*StrArray == NULL)
        *StrArray = calloc(NumStrings, sizeof(char *));
    else {
        puts("StrArray not null, refusing to allocate. Exiting...");
        exit(1);
    }

    /* The Array must be very carefully dereferenced or things will implode. */
    for (iter = 0; iter < NumStrings; iter++)
        (*StrArray)[iter] = NULL;
}


static void noMem()
{
    puts("Failed to allocate enough memory, exiting...");
    exit(EXIT_FAILURE);
}


void myStrFree(int count, ...)
{
    va_list ap;
    int iter;
    char **ptr = NULL;

    va_start(ap, count);
    for (iter = 0; iter < count; iter++) {
        ptr = va_arg(ap, char **);
        if ( *ptr != NULL ) {
            free(*ptr);
            *ptr = NULL;
            ptr = NULL;
        }
    }
    va_end(ap);
}


void freeStrArray(char ***StrArray, int numStrings)
{
    int iter;
    char *str = NULL;

    for (iter = 0; iter < numStrings; iter++) {
        str = (*StrArray)[iter];
        if ( str != NULL ) {
            free(str);
            str = NULL;
        }
    }
    free(*StrArray);
    *StrArray = NULL;
}

