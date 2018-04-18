#include "tree.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STR "\033[32m" "MAX"
#define MIN_STR "\033[31m" "MIN"
#define nputs(STR) fputs((STR), stdout)  /* This saves seconds of typing */

/* static inline void pmaxmin(const struct Node * const node); */


void
print_node(struct Node *node, const bool last)
{
        assert(node->indent - orig_indent < BUFSIZ);

        /*
         * Set the point where the parent node left off to null so we get
         * everything it had up to that point and nothing after.
         */
        *node->indent = '\0';
        nputs(orig_indent);

        if (last)
                nputs("\\-[");
        else
                nputs("+ [");

        /*
         * There must be a better way to do this than constantly calling printf,
         * but this is doubtlessly the laziest way.
         */
        for (int i = 0; i < node->state->len; ++i) {
                if (i < (node->state->len - 1))
                        printf("%d, ", node->state->lst[i]);
                else
                        printf("%d]", node->state->lst[i]);
        }

        if (last) {
                if ((LEAF_ONLY && (node->nchild == 0U)) || (!LEAF_ONLY))
                        printf(" \033[%dm%s\033[0m\n",
                               LEAF_ONLY, node->level ? MAX_STR : MIN_STR);
                else
                        putchar('\n');

                *node->indent++ = ' ';

        } else {
                putchar('\n');
                *node->indent++ = '|';
        }

        *node->indent++ = ' ';
        *node->indent = '\0';
}


#if 0
static inline void
pmaxmin(const struct Node * const node)
{
        if (node->level)
                puts(" \033[1m" MAX_STR "\033[0m");
        else
                puts(" \033[1m" MIN_STR "\033[0m");
}
#endif
