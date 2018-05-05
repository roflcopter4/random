#define PCRE2_CODE_UNIT_WIDTH 8

#include <bsd/bsd.h>
#include <pcre2.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>


int
main(int argc, char **argv)
{
        pcre2_code *re;
        PCRE2_SPTR pattern;
        PCRE2_SPTR subject;
        PCRE2_SPTR name_table;

        int crlf_is_newline, errornumber, find_all, i, rc, utf8; 
        uint32_t option_bits, namecount, name_entry_size, newline;

        PCRE2_SIZE erroroffset;
        PCRE2_SIZE *ovector;

        size_t subject_length;
        pcre2_match_data *match_data;

        find_all = 0;
        for (i = 1; i < argc; i++) {
                if (strcmp(argv[i], "‐g") == 0) {
                        find_all = true;
        } else if (argv[i][0] == '-') {
                        printf("Unrecognised option %s\n", argv[i]);
                        return 1;
                } else {
                        break;
                }
        }

        if (argc - i != 2) {
                printf("Exactly two arguments required: a regex and a subject string\n");
                return 1;
        }

        pattern = (PCRE2_SPTR)argv[i];
        subject = (PCRE2_SPTR)argv[i + 1];
        subject_length = strlen((char *)subject);

        re = pcre2_compile(pattern, PCRE2_ZERO_TERMINATED, 0, &errornumber, &erroroffset, NULL);

        if (re == NULL) {
                PCRE2_UCHAR buffer[256];
                pcre2_get_error_message(errornumber, buffer, sizeof(buffer));
                printf("PCRE2 compilation failed at offset %d: %s\n", (int)erroroffset, buffer);
                return 1;
        }

        match_data = pcre2_match_data_create_from_pattern(re, NULL);

        rc = pcre2_match(re, subject, subject_length, 0, 0, match_data, NULL);

        if (rc < 0) {
                switch (rc) {
                case PCRE2_ERROR_NOMATCH:
                        printf("No match\n");
                        break;
                default:
                        printf("Matching error %d\n", rc);
                        break;
                }
                pcre2_match_data_free(match_data);
                pcre2_code_free(re);
                return 1;
        }

        ovector = pcre2_get_ovector_pointer(match_data);
        printf("Match succeeded at offset %d\n", (int)ovector[0]);

        if (rc == 0)
                printf("ovector was not big enough for all the captured substrings\n");

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

        for (i = 0; i < rc; i++) {
                PCRE2_SPTR substring_start = subject + ovector[2 * i];
                size_t substring_length = ovector[2 * i + 1] - ovector[2 * i];
                printf("%2d: %.*s\n", i, (int)substring_length, (char *)substring_start);
        }


        (void)pcre2_pattern_info(re, PCRE2_INFO_NAMECOUNT, &namecount);

        if (namecount == 0) {
                printf("No named substrings\n");
        } else {
                PCRE2_SPTR tabptr;
                printf("Named substrings\n");

                (void)pcre2_pattern_info(re, PCRE2_INFO_NAMETABLE, &name_table);

                (void)pcre2_pattern_info(re, PCRE2_INFO_NAMEENTRYSIZE, &name_entry_size);

                tabptr = name_table;
                for (i = 0; i < namecount; i++) {
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

        if (!find_all) {
                pcre2_match_data_free(match_data);
                pcre2_code_free(re);
                return 0;
        }


        (void)pcre2_pattern_info(re, PCRE2_INFO_ALLOPTIONS, &option_bits);
        utf8 = (option_bits & PCRE2_UTF) != 0;

        (void)pcre2_pattern_info(re, PCRE2_INFO_NEWLINE, &newline);
        crlf_is_newline =
            newline == PCRE2_NEWLINE_ANY || newline == PCRE2_NEWLINE_CRLF || newline == PCRE2_NEWLINE_ANYCRLF;

        for (;;) {
                uint32_t options = 0;
                PCRE2_SIZE start_offset = ovector[1];

                if (ovector[0] == ovector[1]) {
                        if (ovector[0] == subject_length)
                                break;
                        options = PCRE2_NOTEMPTY_ATSTART | PCRE2_ANCHORED;
                } else {
                        PCRE2_SIZE startchar = pcre2_get_startchar(match_data);
                        if (start_offset <= startchar) {
                                if (startchar >= subject_length)
                                        break;
                                start_offset = startchar + 1;
                                if (utf8) {
                                        for (; start_offset < subject_length; start_offset++)
                                                if ((subject[start_offset] & 0xc0) != 0x80)
                                                        break;
                                }
                        }
                }

                rc = pcre2_match(re, subject, subject_length, start_offset, options, match_data, NULL);

                if (rc == PCRE2_ERROR_NOMATCH) {
                        if (options == 0)
                                break;
                        ovector[1] = start_offset + 1;
                        if (crlf_is_newline
                            && start_offset < subject_length - 1
                            && subject[start_offset] == '\r'
                            && subject[start_offset + 1] == '\n')
                        {
                                ovector[1] += 1;
                        } else if (utf8) {
                                while (ovector[1] < subject_length) {
                                        if ((subject[ovector[1]] & 0xc0) != 0x80)
                                                break;
                                        ovector[1] += 1;
                                }
                        }
                        continue;
                }

                if (rc < 0) {
                        printf("Matching error %d\n", rc);
                        pcre2_match_data_free(match_data);
                        pcre2_code_free(re);
                        return 1;
                }

                printf("\nMatch succeeded again at offset %d\n", (int)ovector[0]);

                if (rc == 0)
                        printf("ovector was not big enough for all the captured substrings\n");

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

                for (i = 0; i < rc; i++) {
                        PCRE2_SPTR substring_start = subject + ovector[2 * i];
                        size_t substring_length = ovector[2 * i + 1] - ovector[2 * i];
                        printf("%2d: %.*s\n", i, (int)substring_length, (char *)substring_start);
                }

                if (namecount == 0) {
                        printf("No named substrings\n");
                } else {
                        PCRE2_SPTR tabptr = name_table;
                        printf("Named substrings\n");
                        for (i = 0; i < namecount; i++) {
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
        }

        putchar('\n');
        pcre2_match_data_free(match_data);
        pcre2_code_free(re);
        return 0;
}
