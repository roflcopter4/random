#include "tree.h"
#include <stdlib.h>
#include <string.h>

#define BUFSZ  4
#define BUFINC 2
#define LAST_IDENT  "  "
#define OTHER_IDENT "| "
#define MAX_STR "\033[32m" "MAX"
#define MIN_STR "\033[31m" "MIN"

#define nputs(STR) fputs((STR), stdout)

static void
do_display(const struct Node *node, char *indent, size_t bufsize, bool last);

/* This was a macro initially, but it's being called twice so perhaps it makes
 * more sense as a function. Maybe. */
static inline void pmaxmin(const struct Node *node);


void
display(struct Node *root)
{       
        char indent[BUFSZ] = " ";
        do_display(root, indent, BUFSZ, true);
}


/* I'm using strcat & memcpy instead of something safer like strlcat/strlcpy
 * only because I am keeping track of the buffer size, so it should be ok. */
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
                strcat(indent, LAST_IDENT);

        } else {
                putchar('\n');
                strcat(indent, OTHER_IDENT);
        }

        bufsize += BUFINC;

        for(int i = 0; i < node->nchild; ++i) {
                char new_indent[bufsize];
                memcpy(new_indent, indent, bufsize * sizeof(char));
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
