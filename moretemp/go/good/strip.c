#include <ctype.h>
#include <err.h>
#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define CC(VAR_)     ((const struct lldata *const)(VAR_))
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


extern void *xmalloc(size_t size);
extern void *xrealloc(void *ptr, size_t size);
static void handle_cstyle(char **buffer, size_t len);
static void handle_python(char **buffer, size_t len);

#define streq(SA,SB) (strcmp((SA), (SB)) == 0)


void
strip_comments(char **buffer, size_t len, const char *lang)
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
                handle_cstyle(buffer, len);
        else if (com.type == PYTHON)
                handle_python(buffer, len);
        else
                errx(1, "This shouldn't be reachable...");
}


/*============================================================================*/
/* C style languages */

#define QUOTE (single_q || double_q)

#define check_quote(CHECK, OTHER)                      \
    do {                                               \
            if (!(OTHER) && !comment) {                \
                    if (CHECK) {                       \
                            if (!escape)               \
                                    (CHECK) = false,   \
                                    transition = true; \
                    } else                             \
                            (CHECK) = true;            \
            }                                          \
            slash = false;                             \
    } while (0)

#define set_comment(TYPE) comment = (TYPE), buf = marker
#define set_slash()       slash = true, marker = buf

enum c_com_type { NONE, BLOCK, LINE };


static void
handle_cstyle(char **buffer, size_t len)
{
        enum c_com_type comment = NONE;
        bool double_q, single_q, slash, escape, transition;
        char *buf, *buf_orig, *marker;
        uint32_t space  = 0;
        const char *pos = *buffer;

        double_q = single_q = slash  = escape = transition = false;
        buf_orig = marker   = buf    = xmalloc(len + 1LLU);

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
                                transition = true;
                        } else if (!double_q) {
                                if (slash && !comment)
                                        set_comment(LINE);
                                else
                                        set_slash();
                        }
                        break;

                case '*':
                        if (!double_q && slash) {
                                if (!comment)
                                        set_comment(BLOCK);
                                slash = false;
                        }
                        break;

                case '\n':
                        if (!escape) {
                                slash = double_q = false;
                                if (comment == LINE)
                                        comment = NONE;
                                if (*(buf - 1) == '\n')
                                        transition = true;
                        }
                        break;

                case '\\': break;
                case '"':  check_quote(double_q, single_q); break;
                case '\'': check_quote(single_q, double_q); break;
                default:   slash = false;
                }

                escape = (*pos == '\\') ? !escape : false;
                space  = (isblank(*pos) &&
                          !(transition = (transition) ? 1 : (*(buf - 1) == '\n'))
                         ) ? space + 1 : 0;

                if (transition)
                        transition = false;
                else if (!comment && !QUOTE && space < 2)
                        *buf++ = *pos;

        } while (*pos++);

        *buf++ = '\0';

        free(*buffer);
        /* vim_buf->len = buf - buf_orig; */
        *buffer = xrealloc(buf_orig, buf - buf_orig);
}

#undef QUOTE
#undef check_quote
#undef setcomment
#undef set_slash


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
                            transition = true;                      \
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
                            transition = true;                      \
                    } else if (!(BB).Q && !comment) {               \
                            if ((AA).Q) {                           \
                                    if (!escape)                    \
                                            (AA).Q = false,         \
                                            transition = true;      \
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
handle_python(char **buffer, size_t len)
{
        enum docstring_e in_docstring = NO_DOCSTRING;
        struct py_quote Single = { false, 0, '\'', SINGLE_DOCSTRING };
        struct py_quote Double = { false, 0, '"',  DOUBLE_DOCSTRING };
        const char *pos        = *buffer;
        uint32_t space         = 0;

        char *buf, *buf_orig;
        bool escape, comment, transition;

        buf    = buf_orig = xmalloc(len + 1LLU);
        escape = comment  = transition = false;

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
                                transition = true;
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
                                transition = true;
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

                if (transition)
                        transition = false;
                else if (!QUOTE && space < 2)
                        *buf++ = *pos;

                escape = (*pos == '\\' ? !escape : false);

        } while (*pos++);

        *buf++ = '\0';

        free(*buffer);
        /* vim_buf->len = buf - buf_orig; */
        *buffer = xrealloc(buf_orig, buf - buf_orig);
}
