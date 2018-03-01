#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mystrlib2.h"

int main()
{
        StringArray data = {NULL, 0};

        get_stdin(&data);

        for (int i = 0; i < data.num; ++i) {
                printf("%s\n", data.arr[i].str);
        }
        
        SA_free(&data);
        return 0;
}
