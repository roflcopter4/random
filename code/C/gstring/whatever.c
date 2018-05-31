#define PCRE2_CODE_UNIT_WIDTH 8
#include <bsd/bsd.h>
#include <malloc.h>
#include <pcre2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PATSIZ 512
#define ARRSIZ(ARR) (sizeof(ARR) / sizeof(*ARR))
#define _substr(INDEX, SUBJECT, OVECTOR) \
        ((char *)((SUBJECT) + (OVECTOR)[(INDEX)*2]))
#define _substrlen(INDEX, OVECTOR) \
        ((int)((OVECTOR)[(2 * (INDEX)) + 1] - (OVECTOR)[2 * (INDEX)]))
typedef unsigned char uchar;

static const char *const arse[] = {
        "Some kinda subject or whatever",
        "You know, like, that thing",
        "12345",
        "Go, to, hell",
};

enum matches_e { ONLY = 1 };

int main(void)
{
        char pat[PATSIZ];
        pcre2_match_data *match_data;
        PCRE2_SIZE erroroffset;
        int errornumber;
        /* normalize_lang(norm_lang, lang, PATSIZ); */

        /* snprintf(pat, PATSIZ, "%s%s%s", PATTERN_PT1, norm_lang, PATTERN_PT2); */
        strlcpy(pat, "((?:(?:\\w+|\\d+),? (?:\\w+| ))|\\d+$)", PATSIZ);
        PCRE2_SPTR pattern = (PCRE2_SPTR)pat;

        pcre2_code *cre = pcre2_compile(pattern, PCRE2_ZERO_TERMINATED, PCRE2_CASELESS,
                                        &errornumber, &erroroffset, NULL);

        if (cre == NULL) {
                PCRE2_UCHAR buf[BUFSIZ];
                pcre2_get_error_message(errornumber, buf, BUFSIZ);
                fprintf(stderr, "PCRE2 compilation failed at offset %d: %s\n",
                       (int)erroroffset, buf);
                exit(1);
        }

        for (size_t iter = 0; iter < ARRSIZ(arse); ++iter) {
                const char *sub = arse[iter];
again:;
                PCRE2_SPTR subject = (PCRE2_SPTR)sub;
                size_t subject_len = (size_t)strlen(sub);

                match_data = pcre2_match_data_create_from_pattern(cre, NULL);
                int rcnt   = pcre2_match(cre, subject, subject_len, 0,
                                         PCRE2_CASELESS, match_data, NULL);

#define substr(INDEX)    _substr(INDEX, subject, ovector)
#define substrlen(INDEX) _substrlen(INDEX, ovector)

                if (rcnt >= 0) {  /* match found */
                        PCRE2_SIZE *ovector = pcre2_get_ovector_pointer(match_data);

                        char tmp[256];
                        size_t len = substrlen(ONLY) + 1;
                        strlcpy(tmp, substr(ONLY), len);
                        printf("%zu: match:  %s\n", iter, tmp);

                        sub += len;
                        pcre2_match_data_free(match_data);

                        if (*sub)
                                goto again;
                } else {
                        printf("%zu: No matches found\n", iter);
                        pcre2_match_data_free(match_data);
                }
        }

        pcre2_code_free(cre);
        return 0;
}
