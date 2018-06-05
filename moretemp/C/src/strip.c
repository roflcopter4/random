#include "neotags.h"
#include <ctype.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#define CC(VAR_)     ((const struct String *const)(VAR_))
#define ARRSIZ(ARR_) (sizeof(ARR_) / sizeof(*(ARR_)))

enum lang_e      { _C_, _CPP_, _CSHARP_, _GO_, _JAVA_, _PYTHON_, };
enum basic_types { C_LIKE, PYTHON };

static const struct lang_s {
        const char *lang;
        const enum basic_types type;
        const enum lang_e lang_id;
} languages[] = {
    { "c",      C_LIKE, _C_      },
    { "cpp",    C_LIKE, _CPP_    },
    { "cs",     C_LIKE, _CSHARP_ },
    { "go",     C_LIKE, _GO_     },
    { "java",   C_LIKE, _JAVA_   },
    { "python", PYTHON, _PYTHON_ },
};

static const struct comment_s {
        const int type;
        const char delim;
} comments[] = {{0, '\0'}, {1, '#'}, {2, ';'}, {3, '"'}};


static void handle_cstyle(struct String *vim_buf);
static void handle_python(struct String *vim_buf);


void
strip_comments(struct String *buffer, const char *lang)
{
        size_t i, size;

        for (i = 0, size = ARRSIZ(languages); i < size; ++i)
                if (streq(lang, languages[i].lang))
                        break;
        if (i == size) {
                warnx("Failed to identify language '%s'.", lang);
                return;
        }

        const struct comment_s com = comments[languages[i].type];

        if (com.type == C_LIKE)
                handle_cstyle(buffer);
        else if (com.type == PYTHON)
                handle_python(buffer);
        else
                errx(1, "This shouldn't be reachable...");
}


/*============================================================================*/
/* C style languages */

#define QUOTE() (single_q || double_q)

#define check_quote(CHECK, OTHER)                      \
    do {                                               \
            if (!(OTHER) && !comment) {                \
                    if (CHECK) {                       \
                            if (!escape)               \
                                    (CHECK) = false,   \
                                    skip = true; \
                    } else                             \
                            (CHECK) = true;            \
            }                                          \
            slash = false;                             \
    } while (0)

#define STR_AND_SIZE(STR_) (STR_), (sizeof(STR_) - 1)

enum c_com_type { NONE, BLOCK, LINE };


static void
handle_cstyle(struct String *vim_buf)
{
        enum c_com_type comment = NONE;
        bool double_q, single_q, slash, escape, skip, /* preproc, */ header;
        char *buf, *buf_orig;
        uint32_t space  = 0;
        const char *pos = vim_buf->s;

        double_q = single_q = slash = escape = skip = /* preproc = */ header = false;
        /* buf_orig = marker   = buf    = xmalloc(vim_buf->len + 1LLU); */
        buf_orig = buf = xmalloc(vim_buf->len + 2LLU);

        if (!*pos)
                errx(1, "Empty vim buffer!");

        /* Add a non-offensive character to the buffer so we never have to worry
         * about going out of bounds when checking 1 character backwards. */
        *buf++ = ' ';

        do {
                switch (*pos) {
                case '/':
                        if (comment == BLOCK && *(pos - 1) == '*') {
                                comment    = NONE;
                                slash      = false;
                                skip = true;
                        } else if (!double_q) {
                                if (slash && !comment) {
                                        comment = LINE;
                                        --buf;
                                } else
                                        slash = true;
                        }
                        break;

                case '*':
                        if (!double_q && slash) {
                                if (!comment) {
                                        comment = BLOCK;
                                        --buf;
                                }
                                slash = false;
                        }
                        break;

                case '\n':
                        if (!escape) {
                                slash = double_q = false;
                                if (comment == LINE)
                                        comment = NONE;
                                if (*(buf - 1) == '\n')
                                        skip = true;
                                /* preproc = false; */
                                header = false;
                        }
                        break;

                case '#':;
                        /* if (!comment && !QUOTE() && *(pos - 1) == '\n')
                                preproc = true; */
                        /* if (strncmp(pos+1, "include", 7) == 0)
                                header = true; */
                        const char *endln = strchr(pos, '\n');
                        if (endln && memmem(pos, endln - pos, STR_AND_SIZE("include")))
                                header = true;
                        slash = false;
                        break;

#if 0
                case '<':
                        if (preproc && !comment && !QUOTE())
                                header = true;
                        slash = false;
                        break;

                case '>':
                        if (header) {
                                header = false;
                                skip   = true;
                        }
                        slash  = false;
                        break;
#endif

                case '\\': break;
                case '"':  check_quote(double_q, single_q); break;
                case '\'': check_quote(single_q, double_q); break;
                default:   slash = false;
                }

                escape = (*pos == '\\') ? !escape : false;
                space  = (isblank(*pos) &&
                          !(skip = (skip) ? 1 : (*(buf - 1) == '\n'))
                         ) ? space + 1 : 0;

                if (skip)
                        skip = false;
                else if (!comment && !QUOTE() && !header && space < 2)
                        *buf++ = *pos;

        } while (*pos++);

        *buf = '\0';

        free(vim_buf->s);
        vim_buf->len = buf - buf_orig - 1LLU;
        vim_buf->s   = xrealloc(buf_orig, vim_buf->len + 1);

        FILE *looog = safe_fopen("/home/bml/blargh.log", "wb");
        fwrite(vim_buf->s, 1, vim_buf->len, looog);
        fclose(looog);
}

#undef QUOTE
#undef check_quote


/*============================================================================*/
/* Python */


#define QUOTE (Single.Q || Double.Q || in_docstring)

#define check_docstring(AA, BB)                                     \
    do {                                                            \
            if (in_docstring) {                                     \
                    if ((AA).cnt == 3)                              \
                            --(AA).cnt;                             \
                    else if (*(pos - 1) == (AA).ch)                 \
                            --(AA).cnt;                             \
                    else                                            \
                            (AA).cnt = 3;                           \
                                                                    \
                    in_docstring = ((AA).cnt != 0) ? (AA).val       \
                                                   : NO_DOCSTRING;  \
                    if (!in_docstring)                              \
                            skip = true;                      \
            } else {                                                \
                    if ((AA).cnt == 0 && !((AA).Q || (BB).Q))       \
                            ++(AA).cnt;                             \
                    else if (*(pos - 1) == (AA).ch)                 \
                            ++(AA).cnt;                             \
                                                                    \
                    in_docstring = ((AA).cnt == 3) ? (AA).val       \
                                                   : NO_DOCSTRING;  \
                                                                    \
                    if (in_docstring) {                             \
                            (AA).Q = (BB).Q = false;                \
                            skip = true;                      \
                    } else if (!(BB).Q && !comment) {               \
                            if ((AA).Q) {                           \
                                    if (!escape)                    \
                                            (AA).Q = false,         \
                                            skip = true;      \
                            } else                                  \
                                    (AA).Q = true;                  \
                    }                                               \
            }                                                       \
    } while (0)


enum docstring_e {
        NO_DOCSTRING,
        SINGLE_DOCSTRING,
        DOUBLE_DOCSTRING
};

struct py_quote {
        bool Q;
        int cnt;
        char ch;
        enum docstring_e val;
};


static void
handle_python(struct String *vim_buf)
{
        enum docstring_e in_docstring = NO_DOCSTRING;
        struct py_quote Single = { false, 0, '\'', SINGLE_DOCSTRING };
        struct py_quote Double = { false, 0, '"',  DOUBLE_DOCSTRING };
        const char *pos        = vim_buf->s;
        uint32_t space         = 0;

        char *buf, *buf_orig;
        bool escape, comment, skip;

        buf    = buf_orig = xmalloc(vim_buf->len + 2LLU);
        escape = comment  = skip = false;

        if (*pos == '\0')
                errx(1, "Empty vim buffer!");

        /* Add a non-offensive character to the buffer so we never have to worry
         * about going out of bounds when checking 1 character backwards. */
        *buf++ = ' ';

        do {
                if (!comment && !QUOTE && !escape && *pos == '#') {
                        comment = true;
                        space   = 0;
                        continue;
                }

                if (comment && *pos != '\n')
                        continue;

                switch (*pos) {
                case '\n':
                        if (*(buf - 1) == '\n')
                                skip = true;
                        comment = false;
                        space = 0;
                        break;

                case '"':
                        if (in_docstring != SINGLE_DOCSTRING)
                                check_docstring(Double, Single);
                        space = 0;
                        break;

                case '\'':
                        if (in_docstring != DOUBLE_DOCSTRING)
                                check_docstring(Single, Double);
                        space = 0;
                        break;

                case '\t':
                case ' ':
                        if (*(buf - 1) == '\n')
                                skip = true;
                        else
                                ++space;
                        break;

                default:
                        space = 0;
                }

                /* If less than 3 of any type of quote appear in a row, reset
                 * the corresponding counter to 0. */
                switch (in_docstring) {
                case SINGLE_DOCSTRING:
                        if (Single.cnt < 3 && *pos != Single.ch)
                                Single.cnt = 3;
                        Double.cnt = 0;
                        break;

                case DOUBLE_DOCSTRING:
                        if (Double.cnt < 3 && *pos != Double.ch)
                                Double.cnt = 3;
                        Single.cnt = 0;
                        break;

                case NO_DOCSTRING:
                        if (Single.cnt > 0 && *pos != Single.ch)
                                Single.cnt = 0;
                        if (Double.cnt > 0 && *pos != Double.ch)
                                Double.cnt = 0;
                        break;

                default: /* not reachable */ abort();
                }

                if (skip)
                        skip = false;
                else if (!QUOTE && space < 2)
                        *buf++ = *pos;

                escape = (*pos == '\\' ? !escape : false);

        } while (*pos++);

        *buf = '\0';

        free(vim_buf->s);
        vim_buf->len = buf - buf_orig - 1LLU;
        vim_buf->s   = xrealloc(buf_orig, vim_buf->len + 1);
}
