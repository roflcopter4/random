#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>
#include "myStrLib.h"

#define CHARSIZE sizeof(char)
#define BUFSIZE 256


void noMem()
{
    puts("Failed to allocate enough memory, exiting...");
    exit(EXIT_FAILURE);
}


void myStrCpy(char **strto, char *strfrom)
{
    size_t len;
    len = strlen(strfrom) + 1;

    /* If string not null, free the memory first. */
    if (*strto != NULL)
        free(*strto);

    /* Allocate just enough memory to hold the whole string and 
     * copy the string to the new buffer. */
    saferCalloc(*strto, len, CHARSIZE);
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
    saferCalloc(*strto, lentot, CHARSIZE);

    /* Copy the first string to the new buffer, hen copy the second string,
     * starting where the first one ended. */
    memcpy(*strto, str1, len1);
    memcpy((*strto + (CHARSIZE * (len1))), str2, len2);
}


void myStrApp(char **strOrig, char *strToAppend)
{
    char *buf = NULL;

    /* If the original string is empty, we can simply copy the other one to it. */
    /* Otherwise we make a copy of the original and then concatenate it with the other. */
    if (*strOrig == NULL)
        myStrCpy(strOrig, strToAppend);
    else     
    {
        myStrCpy(&buf, *strOrig);
        myStrCat(strOrig, buf, strToAppend);
        free(buf);
    }
}


/* ========================================================================= */
/* ========================================================================= */


void myStrInit(int count, ...)
{
    va_list ap;
    int iter;
    char **ptr = NULL;

    va_start(ap, count);
    for (iter = 0; iter < count; ++iter)
    {
        ptr = va_arg(ap, char **);
        if (ptr != NULL)
        {
            *ptr = NULL;
            ptr = NULL;
        }
    }
    va_end(ap);
}


void myStrFree(int count, ...)
/* BROKEN BROKEN BROKEN BROKEN BROKEN BROKEN */
{
    va_list ap;
    int iter;
    char **ptr = NULL;

    va_start(ap, count);
    for (iter = 0; iter < count; ++iter)
    {
        ptr = va_arg(ap, char **);
        if ( *ptr != NULL )
        {
            free(*ptr);
            *ptr = NULL;
            ptr = NULL;
        }
    }
    va_end(ap);
}


/* Returns an array of null string pointers of the designated size. The array
 * must initially be null or this routine will simply exit the program. */
void getStrArray(char ***StrArray, int NumStrings)
{
    int iter;

    if (*StrArray == NULL) {
        saferCalloc(*StrArray, NumStrings, sizeof(char *)); }
    else
    {
        puts("StrArray not null, refusing to allocate. Exiting...");
        exit(1);
    }

    /* The Array must be very carefully dereferenced or things will implode. */
    for (iter = 0; iter < NumStrings; ++iter)
        (*StrArray)[iter] = NULL;
}


void freeStrArray(char ***StrArray, int numStrings)
{
    int iter;
    char *str = NULL;

    for (iter = 0; iter < numStrings; ++iter)
    {
        str = (*StrArray)[iter];
        if ( str != NULL )
        {
            free(str);
            str = NULL;
        }
    }
    free(*StrArray);
    *StrArray = NULL;
}


/* ========================================================================= */
/* ========================================================================= */


int GetAllLines(char ***StrArray, char *filename, int keepEmpty)
{
    int ch, lines, iter, bufIter, strIter;
    char *buf, *all;
    FILE *file;
    buf = all = NULL;
    file = NULL;
    lines = 0;

    file = fopen(filename, "r");
    if (errno)
    {
        char tmp[BUFSIZE];
        sprintf(tmp, "Error opening file '%s'", filename);
        perror(tmp);
        errno = 0;
        return -1;
    }
    else if (file == NULL)
        return -1;

    saferCalloc(buf, BUFSIZE, CHARSIZE);
    iter = 0;

    while ( (ch = fgetc(file)) != EOF )
    {
        if (ch == '\r')
            continue;
        else if (ch == '\n')
        {
            if (iter > 0 || keepEmpty == 1)
            {
                buf[iter] = ch;
                ++lines;
                myStrApp(&all, buf);
                safeFree(buf);
                saferCalloc(buf, BUFSIZE, CHARSIZE);
                iter = 0;
            }
        }
        else
        {
            buf[iter] = ch;
            ++iter;
        }
    }
    fclose(file);

    safeFree(buf);
    getStrArray(StrArray, lines);
    iter = 0;

    for (strIter = 0; strIter < lines; ++strIter)
    {
        saferCalloc(buf, BUFSIZE, CHARSIZE);
        bufIter = 0;
        while ((ch = all[iter]) != '\n')
        {
            buf[bufIter] = ch;
            ++iter;
            ++bufIter;
        }
        myStrCpy(&((*StrArray)[strIter]), buf);
        safeFree(buf);
        ++iter;
    }
    safeFree(all);
    return lines;
}
