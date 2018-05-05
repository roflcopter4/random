#define _GNU_SOURCE
#include <bsd/bsd.h>
#include <stdlib.h>
#include <unistd.h>


int main(void)
{
        int ch, it = 1;
        char orig[BUFSIZ];
        char *buf = orig;

        for (;;) {
                while ((ch = fgetc(stdin)) != '\n')
                        fprintf(stderr, "%c", *buf++ = (char)ch);
                *buf = '\0';
                printf("Hi, you said: %s, and for the %dth time.\n", orig, it++);
                buf = orig;
                fflush(stdout);
        }

        return 0;
}
