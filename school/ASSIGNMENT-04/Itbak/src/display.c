#include "tree.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#define BUFSZ  4
#define BUFINC 2
#define LAST_IDENT  "  "
#define OTHER_IDENT "| "
#define MAX_STR "\033[32m" "MAX"
#define MIN_STR "\033[31m" "MIN"

static void
do_display(const struct Node *node, char *indent, size_t bufsize, bool last);

static inline void pmaxmin(const struct Node *node);


void
display(struct Node *root)
{       
        char indent[BUFSZ] = " ";
        do_display(root, indent, BUFSZ, true);
}


static void
do_display(const struct Node *const node,
           char *indent,
           size_t bufsize,
           bool last)
{
        nputs(indent);

        if (last)
                nputs("\\-[");
        else
                nputs("+ [");

        for (int i = 0; i < node->state->len; ++i)
                if (i < (node->state->len - 1))
                        printf("%d, ", node->state->lst[i]);
                else
                        printf("%d]", node->state->lst[i]);

        if (last) {
                if (LEAF_ONLY) {
                        if (node->nchild == 0)
                                pmaxmin(node);
                        else
                                putchar('\n');
                } else {
                        pmaxmin(node);
                }
                strlcat(indent, LAST_IDENT, bufsize);

        } else {
                putchar('\n');
                strlcat(indent, OTHER_IDENT, bufsize);
        }

        bufsize += BUFINC;

        for(int i = 0; i < node->nchild; ++i) {
                char new_indent[bufsize];
                strlcpy(new_indent, indent, bufsize);
                last = (node->nchild == i + 1);
                do_display(node->child[i], new_indent, bufsize, last);
        }
}


static inline void
pmaxmin(const struct Node *const node)
{
        if (node->level)
                puts(" \033[1m" MAX_STR "\033[0m");
        else
                puts(" \033[1m" MIN_STR "\033[0m");
}


#define NORMAL_INDENT(IT, BUF)       \
        do {                         \
                (BUF)[(IT)++] = '|'; \
                (BUF)[(IT)++] = ' '; \
        } while (0)

#define LAST_INDENT(IT, BUF)         \
        do {                         \
                (BUF)[(IT)++] = ' '; \
                (BUF)[(IT)++] = ' '; \
        } while (0)


void
print_node(struct Node *node, bool last)
{
        assert(node->depth < (BUFSIZ / 2) - 2);
#if 0
        size_t bufsize = (node->depth * 2) + 2;
        char indent[bufsize];

        int i = 1;
        indent[0] = ' ';

        if (node->depth > 0) {
                while (i < (node->depth - 1) * 2)
                        NORMAL_INDENT(i, indent);
                if (last)
                        LAST_INDENT(i, indent);
                else
                        NORMAL_INDENT(i, indent);
        }

        indent[i] = '\0';
        nputs(indent);
#endif
        /* printf("nlists (aka nchild) is %d\n", nchild); */
        /* bool last = (nchild == 0); */

        *node->indent = '\0';
        nputs(orig_indent);

        if (last)
                nputs("\\-[");
        else
                nputs("+ [");

        for (int i = 0; i < node->state->len; ++i) {
                if (i < (node->state->len - 1))
                        printf("%d, ", node->state->lst[i]);
                else
                        printf("%d]", node->state->lst[i]);
        }

        if (last) {
                if ((LEAF_ONLY && (node->nchild == 0U)) || (!LEAF_ONLY)) {
                        /* printf("\nnchild is %u but it says it must be 0??\n", node->nchild); */
                        pmaxmin(node);
                } else
                        putchar('\n');
                *node->indent++ = ' ';
        } else {
                putchar('\n');
                *node->indent++ = '|';
        }

        *node->indent++ = ' ';
        *node->indent = '\0';
#if 0
        orig_indent[node->i] = '\0';
        nputs(orig_indent);

        if (last)
                nputs("\\-[");
        else
                nputs("+ [");

        for (int i = 0; i < node->state->len; ++i) {
                if (i < (node->state->len - 1))
                        printf("%d, ", node->state->lst[i]);
                else
                        printf("%d]", node->state->lst[i]);
        }

        if (last) {
                if ((LEAF_ONLY && node->nchild == 0) || (!LEAF_ONLY))
                        pmaxmin(node);
                else
                        putchar('\n');
                orig_indent[node->i++] = ' ';
        } else {
                putchar('\n');
                orig_indent[node->i++] = '|';
        }

        orig_indent[node->i++] = ' ';
        orig_indent[node->i] = '\0';
#endif
}
