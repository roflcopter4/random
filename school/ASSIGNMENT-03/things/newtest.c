#include <stdio.h>
#include <stdlib.h>
#define BUFMAX 256

static char *get_line();
static int get_int();

int 
main(void)
{
        int a, b;

        a = get_int();
        b = get_int();

        if (a < b)
                printf("%d is less than %d.\n", a, b);
        else if (a == b)
                printf("%d is equal to %d.\n", a, b);
        else
                printf("%d is greater than %d.\n", a, b);

        return 0;
}


static char *
get_line()
{
        int ch, x;
        char *str;
        if ((str = malloc(BUFMAX)) == NULL) {
                fprintf(stderr, "Somehow your computer is out of memory.\n");
                exit(EXIT_FAILURE);
        }

        x = 0;
        while (((ch = getchar()) != '\n') && (ch != EOF) && (x < BUFMAX))
                str[x++] = ch;
        str[x] = '\0';

        return str;
}


static int
get_int()
{
        int ret;
        char *str, *end;
        str = get_line();
        ret = strtol(str, &end, 10);

        if (end[0] != '\0') {
                fprintf(stderr, "Error: Input string '%s' is not a valid integer.\n", str);
                exit(EXIT_FAILURE);
        }

        free(str);
        return ret;
}
