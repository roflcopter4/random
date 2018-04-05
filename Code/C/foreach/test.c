/*#include "foreach_hack.h"*/

#define _GNU_SOURCE

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define FOREACH(ITEM, COUNT, ARRAY, LEN)   \
    for(int _FE_KEEP_ = 1,                 \
            (COUNT)   = 0;                 \
        _FE_KEEP_ && (COUNT) != (LEN);     \
        _FE_KEEP_ = !_FE_KEEP_, ++(COUNT)) \
      for(ITEM = ((ARRAY) + (COUNT)); _FE_KEEP_; _FE_KEEP_ = !_FE_KEEP_)

#define DFOREACH(ITEM, COUNT, ARRAY, LEN)  \
    for(int _FE_KEEP_ = 1,                 \
            (COUNT)   = 0;                 \
        _FE_KEEP_ && (COUNT) != (LEN);     \
        _FE_KEEP_ = !_FE_KEEP_, ++(COUNT)) \
      for(ITEM = *((ARRAY) + (COUNT)); _FE_KEEP_; _FE_KEEP_ = !_FE_KEEP_)


/*===========================================================================*/


#define FIXED_ARR_SIZE(ARRAY) (sizeof(ARRAY) / sizeof(*(ARRAY)))

#define foreach_F(ITEM, ARRAY)                 FOREACH(ITEM, _FE_COUNTER_, ARRAY, FIXED_ARR_SIZE(ARRAY))
#define foreach_L(ITEM, ARRAY, LEN)            FOREACH(ITEM, _FE_COUNTER_, ARRAY, LEN)
#define foreach_DL(ITEM, ARRAY, LEN)          DFOREACH(ITEM, _FE_COUNTER_, ARRAY, LEN)
#define foreach_FC(ITEM, COUNT, ARRAY)         FOREACH(ITEM, COUNT, ARRAY, FIXED_ARR_SIZE(ARRAY))
#define foreach_LC(ITEM, COUNT, ARRAY, LEN)    FOREACH(ITEM, COUNT, ARRAY, LEN)
#define foreach_DLC(ITEM, COUNT, ARRAY, LEN)  DFOREACH(ITEM, COUNT, ARRAY, LEN)


#define SIZ 12

int
main(void)
{
        int myarray[10];
        int n = 1;

        foreach_F (int *x, myarray)
                *x = n++;
        foreach_F (int *x, myarray)
                printf("%d\n", *x);

        int *blargh = malloc(SIZ * sizeof(*blargh));

        foreach_LC (int *x, i, blargh, SIZ)
                *x = i + 1;
        foreach_DL (int x, blargh, SIZ)
                printf("%d\n", x);

        free(blargh);

        return 0;
}
