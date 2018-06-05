#include "neotags.h"
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pthread.h>

#if defined(DOSISH) && !defined(USE_PTHREADS)
#  define USE_PTHREADS
#endif

#ifndef USE_PTHREADS
#  error Pthread support is required.
#endif

#ifdef DOSISH
#  define __CONST__
#  define SEPCHAR ';'
#  include <malloc.h>
#  define vdecl(TYPE_, NAME_, NUM_) \
        TYPE_ *NAME_ = alloca((NUM_) * sizeof(TYPE_))
#else
#  define vdecl(TYPE_, NAME_, NUM_) \
        TYPE_ NAME_[NUM_]
#  define __CONST__ const
#  define SEPCHAR ':'
#endif

#define REQUIRED_INPUT 8
#define CCC(ARG_) ((const char *const *)(ARG_))

struct pdata {
        int threadnum;
        const struct String *vim_buf;
        const char *lang;
        const char *order;
        const char *const *skip;
        const char *const *equiv;
        struct String **lst;
        int num;
};

struct pdata2 {
        const struct String *vim_buf;
        struct String **lst;
        int num;
};

static void search(
        struct StringLst *tags, const struct String *vim_buf,
        const char *lang, const char *order,
        const char *const *skip, const char *const *equiv
);
static char **get_colon_data(char *oarg);
static int ll_cmp(const void *vA, const void *vB);
static void *do_search(void *vdata);
static void *check_buf(void *vdata);

static void tok_search(
        struct StringLst *tags, struct StringLst *vimbuf,
        const char *lang, const char *order,
        const char *const *skip, const char *const *equiv
);
static void * do_tok_search(void *vdata);

static int files_read;

int
main(int argc, char *argv[])
{
        setlocale(LC_NUMERIC, "");
        files_read   = 0;
        program_name = handle_progname(*argv++);
        if (isatty(0))
                errx(1, "This program can't be run manually.");
        if (--argc != REQUIRED_INPUT)
                errx(2, "Error: Wrong number of paramaters (%d, need %d).",
                     argc, REQUIRED_INPUT);
        eputs("Program ID: " PROG_ID "\n");

        char **files   = get_colon_data(*argv++);
        char *ctlang   = *argv++;
        char *vimlang  = *argv++;
        char *order    = *argv++;
        bool strip_com = xatoi(*argv++);
        int64_t nchars = xatoi(*argv++);
        char **skip    = get_colon_data(*argv++);
        char **equiv   = get_colon_data(*argv++);

        /* dump_list(files);
        dump_list(skip);
        dump_list(equiv); */

        struct StringLst tags = {
                .data = malloc(sizeof(*tags.data) * INIT_TAGS),
                .num  = 0,
                .max  = INIT_TAGS
        };

        for (char **ptr = files; *ptr != NULL; ptr += 2) {
                if (!**ptr)
                        ++ptr;
                files_read += getlines(&tags, *ptr, *(ptr + 1));
        }
        if (files_read == 0)
                errx(1, "Error: no files were successfully read.");

        struct String vim_buf = {
            .s    = malloc(nchars + 1),
            .len  = nchars
        };

        fread(vim_buf.s, 1, nchars, stdin);
        vim_buf.s[nchars] = '\0';

        if (strip_com) {
                warnx("Stripping comments...\n");
                strip_comments(&vim_buf, vimlang);
        }

        struct StringLst *toks = tokenize(&vim_buf, vimlang);

        /* search(&tags, &vim_buf, ctlang, order, CCC(skip), CCC(equiv)); */

        tok_search(&tags, toks, ctlang, order, CCC(skip), CCC(equiv));

        /* pointlessly free everything */
        for (int i = 0; i < backup_iterator; ++i)
                free(backup_pointers[i]);
        for (int i = 0; i < tags.num; ++i)
                free(tags.data[i]);
        for (int i = 0; i < toks->num; ++i)
                free(toks->data[i]);

        free_all(equiv, files, skip, tags.data, vim_buf.s, toks->data, toks);

        return 0;
}

/* ========================================================================== */

static char **
get_colon_data(char *oarg)
{
        char sep[2], *tok, *arg = oarg;
        char **data, **odata;
        int num = 0;

        while (*arg != '\0' && (arg = strchr(arg, SEPCHAR)))
                ++num, ++arg;

        data = odata = malloc(sizeof(*data) * (num + 2));
        arg = oarg;
        sep[0] = SEPCHAR;
        sep[1] = '\0';

        while ((tok = strsep(&arg, sep)))
                *data++ = tok;

        if (**(data - 1))
                *data = NULL;
        else
                *(--data) = NULL;

        return odata;
}


static int  /* Comparison function for qsort */
ll_cmp(const void *vA, const void *vB)
{
        int ret;
        const struct String *A = *((struct String *const*)(vA));
        const struct String *B = *((struct String *const*)(vB));

        if (A->kind == B->kind) {
                if (A->len == B->len)
                        ret = memcmp(A->s, B->s, A->len);
                else
                        ret = A->len - B->len;
        } else
                ret = A->kind - B->kind;

        return ret;
}


/* ========================================================================== */


static bool
in_order(const char *const *equiv, const char *order, char *kind)
{
        /* `kind' is actually a pointer to a char, not a C string. */
        for (; *equiv != NULL; ++equiv)
                if (*kind == (*equiv)[0]) {
                        *kind = (*equiv)[1];
                        break;
                }

        return strchr(order, *kind) != NULL;
}


static bool
is_correct_lang(const char *lang, __CONST__ char *match_lang)
{
#ifdef DOSISH
        /* It's a little disgusting to have to strlen every single string in
         * Windows just to get ride of some '\r's, but it must be done. */
        size_t size = strlen(match_lang);
        if (match_lang[size - 1] == '\r')
                match_lang[size - 1] = '\0';
#endif
        if (strCeq(match_lang, lang))
                return true;

        return ((strCeq(lang, "C") || strCeq(lang, "C\\+\\+")) &&
                (strCeq(match_lang, "C") || strCeq(match_lang, "C++")));
}


static bool
skip_tag(const char *const *skip, const char *find)
{
        const char *buf;

        while ((buf = *skip++))
                if (streq(buf, find))
                        return true;

        return false;
}


/*============================================================================*/

#define cpy_ret(DEST_)                                              \
        do {                                                           \
                for (int T = 0; T < num_threads; ++T) {                \
                        if (out[T]->num > 0) {                         \
                                memcpy((DEST_) + offset, out[T]->data, \
                                       out[T]->num * sizeof(*out));    \
                                offset += out[T]->num;                 \
                        }                                              \
                        free(out[T]->data);                            \
                        free(out[T]);                                  \
                }                                                      \
                if (total == 0) {                                      \
                        free((DEST_));                                 \
                        return;                                        \
                }                                                      \
        } while (0)

#define collect()                                          \
        do {                                               \
                for (int th = 0; th < num_threads; ++th) { \
                        void *tmp;                         \
                        pthread_join(tid[th], &tmp);       \
                        out[th] = tmp;                     \
                }                                          \
        } while (0)


static void
search(struct StringLst *tags,
       const struct String *vim_buf,
       const char *lang,
       const char *order,
       const char *const *skip,
       const char *const *equiv)
{
        if (tags->num == 0) {
                warnx("No tags found!");
                return;
        }

        int num_threads = find_num_cpus();
        if (num_threads == 0)
                num_threads = 4;

        uint32_t total, offset, backup_total;
        vdecl(pthread_t, tid, num_threads);
        vdecl(struct StringLst *, out, num_threads);
        warnx("Using %d cpus.", num_threads);

        for (int i = 0; i < num_threads; ++i) {
                struct pdata *tmp = malloc(sizeof *tmp);
                int quot = (int)tags->num / num_threads;
                int num  = (i == num_threads - 1)
                              ? (int)(tags->num - ((num_threads - 1) * quot))
                              : quot;
                *tmp = (struct pdata){ i, vim_buf, lang, order, skip, equiv,
                                       tags->data + (i * quot), num };
                if (pthread_create(tid + i, NULL, &do_search, tmp) != 0)
                        err(1, "pthread_create failed");
        }

        collect();
        total = offset = 0;
        for (int T = 0; T < num_threads; ++T)
                total += out[T]->num;
        backup_total = total;

        struct String **alldata   = malloc(total * sizeof(*alldata));
        /* struct String **filt_data = malloc(total * sizeof(*filt_data)); */

        total = offset = 0;
        for (int T = 0; T < num_threads; ++T)
                total += out[T]->num;
        cpy_ret(alldata);

        qsort(alldata, total, sizeof(*alldata), &ll_cmp);

#if 0
        /* Always display the first item. */
        filt_data[0] = alldata[0];
        int x = 1;

        for (uint32_t i = 1; i < total; ++i)
                if (alldata[i]->len != alldata[i-1]->len
                    || memcmp(alldata[i]->s, alldata[i-1]->s,
                              alldata[i]->len) != 0)
                        filt_data[x++] = alldata[i];

        for (int i = 0; i < num_threads; ++i) {
                struct pdata2 *tmp = malloc(sizeof *tmp);
                int quot = x / num_threads;
                int num = (i == num_threads - 1)
                              ? (x - ((num_threads - 1) * quot))
                              : quot;
                *tmp = (struct pdata2){ vim_buf, filt_data + (i * quot), num };
                if (pthread_create(tid + i, NULL, &check_buf, tmp) != 0)
                        err(1, "pthread_create failed");
        }

        collect();
        total = offset = 0;
        for (int T = 0; T < num_threads; ++T)
                total += out[T]->num;
        cpy_ret(filt_data);
#endif
        /* Always display the first item. */
        printf("%c\n%s\n", alldata[0]->kind, alldata[0]->s);

        for (uint32_t i = 1; i < total; ++i)
                if (alldata[i]->len != alldata[i - 1]->len
                    || memcmp(alldata[i]->s, alldata[i - 1]->s,
                              alldata[i]->len) != 0)
                        printf("%c\n%s\n", alldata[i]->kind, alldata[i]->s);

#if 0
        for (uint32_t i = 0; i < total; ++i)
                printf("%c\n%s\n", filt_data[i]->kind, filt_data[i]->s);
#endif

        for (uint32_t i = 0; i < backup_total; ++i)
                free(alldata[i]);
        free(alldata);
        /* free(filt_data); */
}


static void *
do_search(void *vdata)
{
#  define cur_str (data->lst[i]->s)
        struct pdata *data     = vdata;
        struct String **rdata = malloc(data->num * sizeof(*rdata));
        struct StringLst *ret   = malloc(sizeof *ret);
        *ret = (struct StringLst){ rdata, 0, data->num };

        for (int i = 0; i < data->num; ++i) {
                if (cur_str[0] == '!')
                        continue;

                /* The name is first, followed by two fields we don't need. */
                char *name = strsep(&cur_str, "\t");
                size_t namelen = (cur_str - name - 1LLU);
                cur_str = strchr(cur_str, '\t');
                cur_str = strchr(cur_str, '\t');

                char *tok;
                char *match_lang = NULL;
                char kind = '\0';

                while ((tok = strsep(&cur_str, "\t"))) {
                        /* The 'kind' field is the only one that is 1 character
                         * long, and the 'language' field is prefaced. */
                        if (tok[0] != '\0' && tok[1] == '\0')
                                kind = *tok;
                        else if (strncmp(tok, "language:", 9) == 0)
                                match_lang = tok + 9;
                }

                if (!match_lang || !kind)
                        continue;

                /* Prune tags. Include only those that are:
                 *    1) of a type in the `order' list,
                 *    2) of the correct language,
                 *    3) are not included in the `skip' list, and
                 *    4) are present in the current vim buffer.
                 * If invalid, just move on. */
                if ( in_order(data->equiv, data->order, &kind) &&
                     is_correct_lang(data->lang, match_lang) &&
                    /* !skip_tag(data->skip, name)) */
                    !skip_tag(data->skip, name) &&
                     strstr(data->vim_buf->s, name))
                {
                        struct String *tmp = malloc(sizeof *tmp);
                        tmp->s    = name;
                        tmp->kind = kind;
                        tmp->len  = namelen;
                        ret->data[ret->num++] = tmp;
                }
        }

        free(vdata);
#ifndef DOSISH
        pthread_exit(ret);
#else
        return ret;
#endif
}


static void *
check_buf(void *vdata)
{
        struct pdata2 *data    = vdata;
        struct String **rdata = malloc(data->num * sizeof(*rdata));
        struct StringLst *ret   = malloc(sizeof *ret);
        *ret = (struct StringLst){ rdata, 0, data->num };

        for (int i = 0; i < data->num; ++i)
                if (strstr(data->vim_buf->s, data->lst[i]->s))
                        ret->data[ret->num++] = data->lst[i];
                /* else */
                        /* free(data->lst[i]); */

        free(vdata);
#ifndef DOSISH
        pthread_exit(ret);
#else
        return ret;
#endif
}


/*============================================================================*/


struct tok_pdata {
        const struct StringLst *vim_buf;
        const char *lang;
        const char *order;
        const char *const *skip;
        const char *const *equiv;
        struct String **lst;
        int num;
        int threadnum;
};

FILE *toklog;
/* int *COMP; */

static int
tok_compar(const void *vA, const void *vB/* , void *vC */)
{
        const struct String *const A = *((const struct String *const*)(vA));
        const struct String *const B = *((const struct String *const*)(vB));
        int ret = 0;

        if (A->len == B->len)
                ret = memcmp(A->s, B->s, A->len);
        else
                ret = A->len - B->len;

        /* fprintf(toklog, "ret: %4d  =>  %3zu vs %3zu  --  %s vs %s\n",
                        ret, A->len, B->len, A->s, B->s); */

        return ret;
}


static void
tok_search(struct StringLst *tags,
           struct StringLst *vimbuf,
           const char *lang,
           const char *order,
           const char *const *skip,
           const char *const *equiv)
{
        toklog = safe_fopen("/home/bml/tokcomparlog.log", "wb");
        if (tags->num == 0) {
                warnx("No tags found!");
                return;
        }

        int num_threads = find_num_cpus();
        if (num_threads == 0)
                num_threads = 4;

        uint32_t total, offset, backup_total;
        vdecl(pthread_t, tid, num_threads);
        vdecl(struct StringLst *, out, num_threads);
        warnx("Using %d cpus.", num_threads);

        /* qsort_r(vimbuf->data, vimbuf->num, sizeof(*vimbuf->data), &tok_compar, NULL); */
        qsort(vimbuf->data, vimbuf->num, sizeof(*vimbuf->data), &tok_compar);

        struct StringLst *uniq = malloc(sizeof *uniq);
        *uniq = (struct StringLst){
                .data = malloc(vimbuf->num * sizeof(*uniq->data)),
                .num  = 0,
                .max  = vimbuf->num
        };

        uniq->data[uniq->num++] = vimbuf->data[0];

        for (int i = 1; i < vimbuf->num; ++i)
                if (vimbuf->data[i]->len != vimbuf->data[i-1]->len
                    || memcmp(vimbuf->data[i]->s,
                              vimbuf->data[i-1]->s,
                              vimbuf->data[i]->len) != 0)
                        uniq->data[uniq->num++] = vimbuf->data[i];

        for (int i = 0; i < uniq->num; ++i)
                fprintf(toklog, "%s\n", uniq->data[i]->s);

        for (int i = 0; i < num_threads; ++i) {
                struct tok_pdata *tmp = malloc(sizeof *tmp);
                int quot = (int)tags->num / num_threads;
                int num  = (i == num_threads - 1)
                              ? (int)(tags->num - ((num_threads - 1) * quot))
                              : quot;

                *tmp = (struct tok_pdata){ uniq, lang, order, skip, equiv,
                                           tags->data + (i * quot), num, i };

                if (pthread_create(tid + i, NULL, &do_tok_search, tmp) != 0)
                        err(1, "pthread_create failed");
        }

        collect();
        total = offset = 0;
        for (int T = 0; T < num_threads; ++T)
                total += out[T]->num;
        backup_total = total;

        struct String **alldata = malloc(total * sizeof(*alldata));

        total = offset = 0;
        for (int T = 0; T < num_threads; ++T)
                total += out[T]->num;
        cpy_ret(alldata);

        qsort(alldata, total, sizeof(*alldata), &ll_cmp);

        printf("%c\n%s\n", alldata[0]->kind, alldata[0]->s);

        for (uint32_t i = 1; i < total; ++i)
                if (alldata[i]->len != alldata[i - 1]->len
                    || memcmp(alldata[i]->s, alldata[i - 1]->s,
                              alldata[i]->len) != 0)
                        printf("%c\n%s\n", alldata[i]->kind, alldata[i]->s);

        for (uint32_t i = 0; i < backup_total; ++i)
                free(alldata[i]);
        free(alldata);
        free(uniq->data);
        free(uniq);
        fclose(toklog);
}


static void *
do_tok_search(void *vdata)
{
        struct tok_pdata *data = vdata;

        struct String **rdata = malloc(data->num * sizeof(*rdata));
        struct StringLst *ret   = malloc(sizeof *ret);
        *ret = (struct StringLst){ rdata, 0, data->num };

        for (int i = 0; i < data->num; ++i) {
                if (cur_str[0] == '!')
                        continue;

                /* The name is first, followed by two fields we don't need. */
                char *name = strsep(&cur_str, "\t");
                size_t namelen = (cur_str - name - 1LLU);
                cur_str = strchr(cur_str, '\t');
                cur_str = strchr(cur_str, '\t');

                char *tok;
                char *match_lang = NULL;
                char kind = '\0';

                while ((tok = strsep(&cur_str, "\t"))) {
                        /* The 'kind' field is the only one that is 1 character
                         * long, and the 'language' field is prefaced. */
                        if (tok[0] != '\0' && tok[1] == '\0')
                                kind = *tok;
                        else if (strncmp(tok, "language:", 9) == 0)
                                match_lang = tok + 9;
                }

                if (!match_lang || !kind)
                        continue;

                struct String name_s_tmp = {
                        .s   = name,
                        .len = namelen
                };
                struct String *name_s = &name_s_tmp;

                /* Prune tags. Include only those that are:
                 *    1) of a type in the `order' list,
                 *    2) of the correct language,
                 *    3) are not included in the `skip' list, and
                 *    4) are present in the current vim buffer.
                 * If invalid, just move on. */
                if (in_order(data->equiv, data->order, &kind) &&
                    is_correct_lang(data->lang, match_lang) &&
                    /* !skip_tag(data->skip, name)) */
                    !skip_tag(data->skip, name) &&
                    bsearch(&name_s,
                            data->vim_buf->data,
                            data->vim_buf->num,
                            sizeof(*data->vim_buf->data),
                            &tok_compar))
                {
                        struct String *tmp = malloc(sizeof *tmp);
                        tmp->s    = name;
                        tmp->kind = kind;
                        tmp->len  = namelen;
                        ret->data[ret->num++] = tmp;
                }
        }

        free(vdata);
#ifndef DOSISH
        pthread_exit(ret);
#else
        return ret;
#endif
}
