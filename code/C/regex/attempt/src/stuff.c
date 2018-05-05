static int
things(struct str_vec *taglist)
{
        pcre2_code *cre;
        PCRE2_SPTR pattern;
        PCRE2_SPTR subject;
        /* PCRE2_SPTR name_table; */

        /* int crlf_is_newline; */
        int errornumber;
        int rcnt;

        uint32_t namecount;
        /* uint32_t name_entry_size; */
        /* uint32_t newline; */

        PCRE2_SIZE erroroffset;
        PCRE2_SIZE *ovector;

        size_t subject_length;
        pcre2_match_data *match_data;

        char pat[PATSIZ];
        (void)snprintf(pat, PATSIZ, "%s%s%s", PATTERN_PT1, LANG, PATTERN_PT2);
        puts(pat);
        pattern = (PCRE2_SPTR)pat;

        cre = pcre2_compile(pattern,
                            PCRE2_ZERO_TERMINATED,
                            PCRE2_CASELESS,
                            &errornumber,
                            &erroroffset,
                            NULL);

        /* Compilation failed: print the error message and exit. */
        if (cre == NULL) {
                PCRE2_UCHAR buffer[256];
                pcre2_get_error_message(errornumber, buffer, sizeof(buffer));
                xerr(1, "PCRE2 compilation failed at offset %d: %s\n", (int)erroroffset, buffer);
        }

        for (uint32_t iter = 0; iter < taglist->num; ++iter) {
                
                if (taglist->s[iter][0] == '!' || taglist->s[iter][0] == '\0')
                        continue;

                subject = (PCRE2_SPTR)taglist->s[iter];
                subject_length = (size_t)(taglist->slen[iter] - 1);

                /* Using this function ensures that the block is exactly the right size for
                the number of capturing parentheses in the pattern. */
                match_data = pcre2_match_data_create_from_pattern(cre, NULL);
                rcnt = pcre2_match(cre, subject, subject_length,
                                   0,              /* start at offset 0 in the subject */
                                   PCRE2_CASELESS, /* default options */
                                   match_data,     /* block for storing the result */
                                   NULL);          /* use default match context */

                /* Matching failed: handle error cases */
                if (rcnt < 0) {
                        pcre2_match_data_free(match_data);
                        continue;
                }

                /* Match succeded. Get a pointer to the output vector, where string offsets are stored. */
                ovector = pcre2_get_ovector_pointer(match_data);

                /* printf("%s\n", cur); */
                printf("Match succeeded at offset %d\n", (int)ovector[0]);

#if 0
                /* The output vector wasn’t big enough. This should not happen, because we used
                pcre2_match_data_create_from_pattern() above. */
                if (rcnt == 0)
                        printf("ovector was not big enough for all the captured substrings\n");
#endif

#define substring(IND) ((char *)(subject + ovector[(IND) * 2]))

                /* Show substrings stored in the output vector by number. Obviously, in a real
                application you might want to do things other than print them. */
                for (int i = 0; i < rcnt; i++) {
                        /* PCRE2_SPTR substring_start = subject + ovector[2 * i]; */
                        size_t substring_length = ovector[2 * i + 1] - ovector[2 * i];
                        /* printf("%2d: %.*s\n", i, (int)substring_length, (char *)substring_start); */
                        printf("%2d: %.*s\n", i, (int)substring_length, substring(i));
                }


#if 0
                /* See if there are any named substrings, and if so, show them by name. First
                we have to extract the count of named parentheses from the pattern. */
                (void)pcre2_pattern_info(cre,                   /* the compiled pattern */
                                         PCRE2_INFO_NAMECOUNT, /* get the number of named substrings */
                                         &namecount);          /* where to put the answer */

                if (namecount == 0) {
                        printf("No named substrings\n");
                } else {
                        PCRE2_SPTR tabptr;
                        printf("Named substrings\n");

                        /* Before we can access the substrings, we must extract the table for
                        translating names to numbers, and the size of each entry in the table. */
                        (void)pcre2_pattern_info(cre,                  /* the compiled pattern */
                                                 PCRE2_INFO_NAMETABLE, /* address of the table */
                                                 &name_table);         /* where to put the answer */

                        (void)pcre2_pattern_info(cre,                      /* the compiled pattern */
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
#endif
                pcre2_match_data_free(match_data);
        }

        pcre2_code_free(cre);
        return 0;
}
