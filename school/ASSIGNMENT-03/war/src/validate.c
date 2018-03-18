#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "war.h"

#define prarray2(X)                           \
do {                                             \
        for (int i = 0; i < 52; ++i)       \
                printf("%s\n", (X)[i]);          \
} while (0)


int compare(const void* a, const void* b);

void
validate_data(struct StringArray *data)
{
        assert(data->num == 52);

        /*print_array(data);*/
        char *str_array[52];
        /*char **str_array;*/
        /*str_array = calloc(52, sizeof(char *));*/
        /*size_t siz = 0;*/
        for (int i = 0; i < 52; ++i) {
                str_array[i] = malloc(data->arr[i].len);
                memcpy(str_array[i], data->arr[i].str, data->arr[i].len);
                /*puts(str_array[i]);*/
                /*printf("%lu", strlen(data->arr[i].str));*/
                /*str_array[i] = data->arr[i].str;*/
                /*siz = (siz < data->arr[i].len) ? data->arr[i].len : siz;*/
                /*printf("%lu\n", data->arr[i].len);*/
        }
        /*printf("%lu\n", siz*sizeof(char));*/

        /*prarray2(data, str_array);*/
        qsort(str_array, 52, 3, compare);
        /*qsort(data->arr, 52, 4, compare);*/
        /*puts(data->arr[3].str);*/

        /*for (int i = 0; i < 52; ++i)*/
                /*free(str_array[i]);*/
        /*puts(str_array[1]);*/
        /*prarray2(data, str_array);*/
        /*print_array(data);*/
        prarray2(str_array);
}


void
print_array(struct StringArray *data)
{
        for (int i = 0; i < data->num; ++i) {
                printf("%s\n", data->arr[i].str);
        }
}


int
compare(const void *a, const void *b)
{
        const char *pa = (const char**)a;
        const char *pb = (const char**)b;
        return strcmp(pa,pb);
}
