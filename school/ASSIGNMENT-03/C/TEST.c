#define _GNU_SOURCE
#include <bsd/bsd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUMSTRINGS 10

int cmp(const void* a, const void* b)
{
        const char *pa = *(char * const *)a;
        const char *pb = *(char * const *)b;
        /*char *pa = * (char * const *)a*/
        /*return strncmp(pa, pb, 1);*/
        return strcmp(pa, pb);
        /*return strncmp(* (char * const *) a, * (char * const *) b, 6);*/
}


int main(void)
{
        char **test = malloc(NUMSTRINGS * sizeof(char *));
        char *tmp;

        for (int i = 0; i < NUMSTRINGS; ++i) {
                test[i] = malloc(BUFSIZ);
                asprintf(&tmp, "%3d%3d", i, i*i);
                strlcpy(test[i], tmp, BUFSIZ);
                free(tmp);
        }

        for (int i = 0; i < NUMSTRINGS; ++i) {
                puts(test[i]);
        }

        qsort(test, NUMSTRINGS, sizeof(char *), cmp);

        for (int i = 0; i < NUMSTRINGS; ++i) {
                puts(test[i]);
                free(test[i]);
        }

        free(test);
}
