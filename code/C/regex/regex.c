#define PCRE2_CODE_UNIT_WIDTH 8
#include <bsd/bsd.h>
#include <limits.h>
#include <pcre2.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>


int
main(int argc, char **argv, char **env)
{
        pcre2_code *re;
        PCRE2_SPTR pattern;
        PCRE2_SPTR subject = (const unsigned char *)u"the basteen 1224 arse";
        PCRE2_SPTR name_table;

        int crlf_is_newline, errornumber, find_all, i, rc, utf8; 
        uint32_t option_bits, namecount, name_entry_size, newline;

        PCRE2_SIZE erroroffset;
        PCRE2_SIZE *ovector;

        /* const unsigned char *input =  */
        size_t subject_length = strlen(subject);
        pcre2_match_data *match_data;

        /* unsigned char *str = (unsigned char *)u"\\d+"; */
        re = pcre2_compile("\d+", PCRE2_ZERO_TERMINATED, 0, &errornumber, &erroroffset, NULL);

        rc = pcre2_match(re, subject, subject_length, 0, 0, match_data, NULL);

        ovector = pcre2_get_ovector_pointer(match_data);
        printf("Idunno: %d -> %zu\n", rc, *ovector);
}
