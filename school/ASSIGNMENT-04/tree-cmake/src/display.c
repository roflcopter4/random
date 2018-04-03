#include "tree.h"
#include <bsd/bsd.h>
#include <stdlib.h>
#include <string.h>

/*#define BUFSZ 2048*/
#define BUFSZ 5

#define nputs(STR) fputs((STR), stdout)

#define pmaxmin(NODE)                 \
        do {                          \
                if ((NODE)->level)    \
                        puts(" MAX"); \
                else                  \
                        puts(" MIN"); \
        } while (0)

static void do_display(struct Node *node, char *indent, size_t bufsize, bool last);


void
display(struct Node *root)
{       
        nputs(" \\-[");
        for (int i = 0; i < root->state.len; ++i)
                if (i < root->state.len - 1)
                        printf("%d, ", root->state.lst[i]);
                else
                        printf("%d]", root->state.lst[i]);
        pmaxmin(root);

        for(int i = 0; i < root->nchild; ++i) {
                bool last = (root->nchild == i + 1);
                char indent[BUFSZ] = "  ";

                do_display(root->child[i], indent, BUFSZ, last);
        }
}


static void
do_display(struct Node *node, char *indent, size_t bufsize, bool last)
{
        putchar(' ');
        nputs(indent);

        if (last)
                nputs("\\-[");
        else
                nputs("+ [");

        for (int i = 0; i < node->state.len; ++i)
                if (i < node->state.len - 1)
                        printf("%d, ", node->state.lst[i]);
                else
                        printf("%d]", node->state.lst[i]);

        if (last) {
                /*pmaxmin(node);*/
                if (node->nchild == 0)
                        pmaxmin(node);
                else
                        putchar('\n');
                strlcat(indent, "  ", bufsize);
        } else {
                putchar('\n');
                strlcat(indent, "| ", bufsize);
        }

        bufsize += 2;
        for(int i = 0; i < node->nchild; ++i) {
                char new_indent[bufsize];
                strlcpy(new_indent, indent, bufsize);
                last = (node->nchild == i + 1);
                do_display(node->child[i], new_indent, bufsize, last);
        }
}
