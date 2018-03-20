#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>
#include "mystrlib2.h"

#define CHARSIZE sizeof(char)
#define BUFSIZE 256


void my_string_cpy(String *strto, char *strfrom)
{
    size_t len;
    len = strlen(strfrom) + 1;

    /* If string not null, free the memory first. */
    if (strto->str != NULL)
        free(strto->str);

    /* Allocate just enough memory to hold the whole string and 
     * copy the string to the new buffer. */
    saferCalloc(strto->str, len, CHARSIZE);
    memcpy(strto->str, strfrom, len);
    strto->len = len;
}

void my_stringObj_cpy(String *strto, String *strfrom)
{
    /* If string not null, free the memory first. */
    if (strto->str != NULL)
        free(strto->str);
    if (strfrom->str == NULL)
        exit(EXIT_FAILURE);

    /* Allocate just enough memory to hold the whole string and 
     * copy the string to the new buffer. */
    saferCalloc(strto->str, strfrom->len, CHARSIZE);
    memcpy(strto->str, strfrom->str, strfrom->len);
    strto->len = strfrom->len;
}


/* ========================================================================= */
/* ========================================================================= */


void init_string_array(StringArray *str_array)
{
    int iter;

    if (str_array->arr == NULL) 
    {
        saferCalloc(str_array->arr, str_array->num, sizeof(String));
    }
    else
    {
        puts("StrArray not null, refusing to allocate. Exiting...");
        exit(EXIT_FAILURE);
    }

    /* The Array must be very carefully dereferenced or things will implode. */
    for (iter = 0; iter < str_array->num; ++iter)
        str_array->arr[iter].str = NULL;
}


void free_string_array(StringArray *str_array)
{
    int iter;
    char *str = NULL;

    for (iter = 0; iter < str_array->num; ++iter)
    {
        if ( str != NULL )
        {
            free(str_array->arr[iter].str);
            str = NULL;
        }
    }
    free(str_array->arr);
    str_array->arr = NULL;
}

/* ========================================================================= */
/* ========================================================================= */


int get_all_lines(StringArray *str_array, char *filename)
{
    int ch, it, x;
    FILE *fp;

    fp = fopen(filename, "r");
    if (errno)
    {
        char tmp[BUFSIZE];
        sprintf(tmp, "Error opening file '%s'", filename);
        perror(tmp);
        errno = 0;
        return -2;
    }
    else if (fp == NULL)
        return -3;
    
    str_array->num = 0;
    while ((ch = getc(fp)) != EOF)
    {
        if (ch == '\n')
            ++str_array->num;
    }

    fclose(fp);
    fp = fopen(filename, "r");
    /*getStrArray(&(str_array->arr), str_array->num);*/
    init_string_array(str_array);

    for(it = 0; it < str_array->num; ++it)
    {
        if ((x = my_fgetline(&(str_array->arr[it].str), fp)) < 0) {
            noMem();
        } else {
            ++x;
            str_array->arr[it].len = (size_t)(x);
        }
    }

    fclose(fp);
    return 0;
}

