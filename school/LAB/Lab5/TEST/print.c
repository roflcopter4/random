#include <stdio.h>
#include <stdlib.h>

int main(void)
{
        char *str = "780-238-6515";
        for (long int i = 0; i < 10000000000000; ++i) {
                puts(str);
        }

        return 0;
}
