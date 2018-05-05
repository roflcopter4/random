#define PCRE2_CODE_UNIT_WIDTH 8

#include <bsd/bsd.h>
#include <pcre2.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define LANG "C"

int main(void)
{
        pcre2_code *re;
        PCRE2_SPTR pattern; /* PCRE2_SPTR is a pointer to unsigned code units of */
        PCRE2_SPTR subject; /* the appropriate width (in this case, 8 bits). */
        PCRE2_SPTR name_table;

        int crlf_is_newline;
        int errornumber;
        int rc;
        int utf8;

        uint32_t option_bits;
        uint32_t namecount;
        uint32_t name_entry_size;
        uint32_t newline;

        PCRE2_SIZE erroroffset;
        PCRE2_SIZE *ovector;

        size_t subject_length;
        pcre2_match_data *match_data;

        /* As pattern and subject are char arguments, they can be straightforwardly
        cast to PCRE2_SPTR as we are working in 8‐bit code units. */

        /*        
           pattern = re.compile(
               b'(?:^|\n)(?P<name>[^\t]+)\t(?P<file>[^\t]+)\t\/(?P<cmd>.+)\/;"\t(?P<kind>\w)\tlanguage:(?P<lang>' +
               bytes(lang, 'utf8') + b'(?:\w+)?)', re.IGNORECASE
           )
         */

        /* pattern = (PCRE2_SPTR)("(?:^|\\n)(?P<name>[^\\t]+)\\t(?P<file>[^\\t]+)\\t\\/(?P<cmd>.+)\\/;\"\\t(?P<kind>\\w)\\tlanguage:(?P<lang>" "c" "(?:\\w+)?)"); */

        pattern = (PCRE2_SPTR)("(?:^|\\n)([^\\t]+)\\t([^\\t]+)\\t\\/(.+)\\/;\"\\t(\\w)\\tlanguage:(" LANG "(?:\\w+)?)");

        /* pattern = (PCRE2_SPTR)"\\d{1,2}hi (cunt|whore|bitch)$"; */
        subject = (PCRE2_SPTR)"main	/home/bml/random/Code/C/regex/tst2.c	/^int main(void)$/;\"	f	language:C	typeref:typename:int";
        subject_length = strlen((char *)subject);

        /*************************************************************************
         * Now we are going to compile the regular expression pattern, and handle *
         * any errors that are detected.                                          *
         *************************************************************************/

        re = pcre2_compile(pattern,               /* the pattern */
                           PCRE2_ZERO_TERMINATED, /* indicates pattern is zero‐terminated */
                           PCRE2_CASELESS,        /* default options */
                           &errornumber,          /* for error number */
                           &erroroffset,          /* for error offset */
                           NULL);                 /* use default compile context */

        /* Compilation failed: print the error message and exit. */

        if (re == NULL) {
                PCRE2_UCHAR buffer[256];
                pcre2_get_error_message(errornumber, buffer, sizeof(buffer));
                printf("PCRE2 compilation failed at offset %d: %s\n", (int)erroroffset, buffer);
                return 1;
        }

        /*************************************************************************
         * If the compilation succeeded, we call PCRE again, in order to do a     *
         * pattern match against the subject string. This does just ONE match. If *
         * further matching is needed, it will be done below. Before running the  *
         * match we must set up a match_data block for holding the result.        *
         *************************************************************************/

        /* Using this function ensures that the block is exactly the right size for
        the number of capturing parentheses in the pattern. */

        match_data = pcre2_match_data_create_from_pattern(re, NULL);

        rc = pcre2_match(re,             /* the compiled pattern */
                         subject,        /* the subject string */
                         subject_length, /* the length of the subject */
                         0,              /* start at offset 0 in the subject */
                         PCRE2_CASELESS, /* default options */
                         match_data,     /* block for storing the result */
                         NULL);          /* use default match context */

        /* Matching failed: handle error cases */

        if (rc < 0) {
                switch (rc) {
                case PCRE2_ERROR_NOMATCH:
                        printf("No match\n");
                        break;
                /*
                Handle other special cases if you like
                */
                default:
                        printf("Matching error %d\n", rc);
                        break;
                }
                pcre2_match_data_free(match_data); /* Release memory used for the match */
                pcre2_code_free(re);               /* data and the compiled pattern. */
                return 1;
        }

        /* Match succeded. Get a pointer to the output vector, where string offsets are
        stored. */

        ovector = pcre2_get_ovector_pointer(match_data);
        printf("Match succeeded at offset %d\n", (int)ovector[0]);


        /*************************************************************************
         * We have found the first match within the subject string. If the output *
         * vector wasn’t big enough, say so. Then output any substrings that were *
         * captured.                                                              *
         *************************************************************************/

        /* The output vector wasn’t big enough. This should not happen, because we used
        pcre2_match_data_create_from_pattern() above. */

        if (rc == 0)
                printf("ovector was not big enough for all the captured substrings\n");

        /* We must guard against patterns such as /(?=.\K)/ that use \K in an assertion
        to set the start of a match later than its end. In this demonstration program,
        we just detect this case and give up. */

        if (ovector[0] > ovector[1]) {
                printf("\\K was used in an assertion to set the match start after its end.\n"
                       "From end to start the match was: %.*s\n",
                       (int)(ovector[0] - ovector[1]),
                       (char *)(subject + ovector[1]));
                printf("Run abandoned\n");
                pcre2_match_data_free(match_data);
                pcre2_code_free(re);
                return 1;
        }

        /* Show substrings stored in the output vector by number. Obviously, in a real
        application you might want to do things other than print them. */

        for (int i = 0; i < rc; i++) {
                PCRE2_SPTR substring_start = subject + ovector[2 * i];
                size_t substring_length = ovector[2 * i + 1] - ovector[2 * i];
                printf("%2d: %.*s\n", i, (int)substring_length, (char *)substring_start);
        }


        /* See if there are any named substrings, and if so, show them by name. First
        we have to extract the count of named parentheses from the pattern. */

        (void)pcre2_pattern_info(re,                   /* the compiled pattern */
                                 PCRE2_INFO_NAMECOUNT, /* get the number of named substrings */
                                 &namecount);          /* where to put the answer */

        if (namecount == 0) {
                printf("No named substrings\n");
        } else {
                PCRE2_SPTR tabptr;
                printf("Named substrings\n");

                /* Before we can access the substrings, we must extract the table for
                translating names to numbers, and the size of each entry in the table. */

                (void)pcre2_pattern_info(re,                   /* the compiled pattern */
                                         PCRE2_INFO_NAMETABLE, /* address of the table */
                                         &name_table);         /* where to put the answer */

                (void)pcre2_pattern_info(re,                       /* the compiled pattern */
                                         PCRE2_INFO_NAMEENTRYSIZE, /* size of each entry in the table */
                                         &name_entry_size);        /* where to put the answer */

                /* Now we can scan the table and, for each entry, print the number, the name,
                and the substring itself. In the 8‐bit library the number is held in two
                bytes, most significant first. */

                tabptr = name_table;
                for (unsigned i = 0; i < namecount; i++) {
                        int n = (tabptr[0] << 8) | tabptr[1];
                        printf("(%d) %*s: %.*s\n",
                               n,
                               name_entry_size - 3,
                               tabptr + 2,
                               (int)(ovector[2 * n + 1] - ovector[2 * n]),
                               subject + ovector[2 * n]);
                        tabptr += name_entry_size;
                }
        }


        putchar('\n');
        pcre2_match_data_free(match_data);
        pcre2_code_free(re);
        return 0;
}
