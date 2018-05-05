#include "neotags.h"
#include <stdio.h>
#include <stdlib.h>


int
main(int argc, char **argv)
{
        if (argc < 3)
                exit(1);

        program_name  = *argv++;
        int64_t chars = xatoi(*argv++);
        char *file    = malloc(chars * sizeof(*file));
        char *find;

        for (int i = 0; i < chars; ++i)
                file[i] = getchar();

        while ((find = *argv++) != NULL) {
                if (strstr(file, find)) 
                        puts("True");
                else 
                        puts("False");
        }

        free(file);
        return 0;
}


int64_t
__xatoi(const char *str, bool strict)
{
        char *endptr;
        int64_t val = strtol(str, &endptr, 10);

        if ((endptr == str) || (strict && *endptr != '\0'))
                xeprintf(30, "Invalid integer '%s'.\n", str);

        return val;
}
