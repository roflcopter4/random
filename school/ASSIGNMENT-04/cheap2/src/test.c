#include "tree.h"
#include <limits.h>
#include <stdlib.h>
#include <string.h>

#define ARRLEN 300000000


int
main(int argc, char **argv)
{
        if (argc == 1)
                exit(1);

        uint *arr1, *arr3;
        bool x = true;
        arr1   = malloc(ARRLEN * sizeof(*arr1));
        arr3   = malloc(ARRLEN * sizeof(*arr3));

        for (int i = 0; i < ARRLEN; ++i) {
                arr1[i] = i;
                /*arr3[i] = i + 1;*/
                arr3[i] = i;
        }

        if (strcmp(argv[1], "for") == 0) {
                for (uint i = 0; i < ARRLEN; ++i)
                        if (arr1[i] != arr3[i]) {
                                x = false;
                                break;
                        }
                printf("For loop result: %d\n", x);

        } else if (strcmp(argv[1], "mem") == 0) {
                x = memcmp(arr1, arr3, ARRLEN * sizeof(uint)) == 0;
                printf("Memcmp result: %d\n", x);
        }

        free(arr1);
        free(arr3);
        return 0;
}
