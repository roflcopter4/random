#define _GNU_SOURCE
#include <fcntl.h>
#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <ustr.h>
#define S(STRING__) ustr_cstr(STRING__)

int
main(void)
{
        Ustr *s1 = ustr_dup_cstr("Hello");
        printf("So %zu and also %zu\n", ustr_len(s1), ustr_size_alloc(s1));
        ustr_set_fmt(&s1, "Some kidna stupid format or like whatever 0%3o", 32109);
        puts(S(s1));
        printf("So %zu and also %zu\n", ustr_len(s1), ustr_size_alloc(s1));

        if (!ustr_io_getfilename(&s1, "BSDmakefile"))
                perror("Sigh."), exit(1);

        puts(S(s1));
        /* ustr_free(s1); */

        Ustr *s2 = ustr_dup_empty();
        (void)ustr_io_getfilename(&s2, "/home/bml/.vimrc");
        puts(S(s2));


        ustr_free(s1);
        ustr_free(s2);

        regex_t bla;
        regcomp(&bla, "\\d+(\\w+)$", REG_EXTENDED);
        size_t n = 0;
        regmatch_t match[5];
        char cs[] = "124523hellofaggot";
        int ret = regexec(&bla, cs, 1, match, 0);

        printf("Ret -> %d, match -> \n", ret/*, match[0] */);
}
