#include <stdio.h>
#define INC_STRINGS(CUR_) ((CUR_) <<= 1)
int
main(void)
{
        int x = 4096;
        printf("That num again is %d -> %d\n", INC_STRINGS(x), 2);
        return 0;
}
