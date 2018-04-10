#include "fool.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#define MAXCHILD 2

static inline void setup_node(struct expression *node);
static inline void check_max(struct expression *node);


void
destroy_tree(struct expression *node)
{
        for (uint i = 0; i < node->maxchild; ++i) {
                if (i < node->nchild && node->child[i]->tag == EXPR)
                        destroy_tree(node->child[i]->u.expr);
                free(node->child[i]);
        }

        free(node->child);
        free(node);
}


void
destroy_child(struct Child *child)
{
        if (child->tag == EXPR)
                destroy_tree(child->u.expr);
        free(child);
}


struct expression *
init_tree()
{
        struct expression *root = xmalloc(sizeof *root);
        setup_node(root);
        root->in_paren = false;
        return root;
}


void
_add_operand(struct expression *node, const double num)
{
        check_max(node);
        node->child[node->nchild]->u.ans = num;
        node->child[node->nchild++]->tag = ANS;
}


void
insert_operand(struct expression *node, const double num, const uint index)
{
        assert(index < node->nchild);
        node->child[index] = xmalloc(sizeof **node->child);
        node->child[index]->tag    = ANS;
        node->child[index]->u.ans  = num;
}


struct expression *
add_expr(struct expression *parent, const bool in_paren)
{
        struct expression *node = xmalloc(sizeof *node);
        setup_node(node);
        node->in_paren = in_paren;

        check_max(parent);
        parent->child[parent->nchild]->u.expr = node;
        parent->child[parent->nchild++]->tag  = EXPR;

        return node;
}

struct Child *
replace_node(struct expression *node, const double num)
{
        destroy_tree(node);
        struct Child *child = xmalloc(sizeof *child);
        child->tag = ANS;
        child->u.ans = num;
        return child;
}


void
add_operator(struct expression *node, const int ch)
{
        const char *buf = "+-*/%^";
        if (index(buf, ch) != NULL) {
                if (node->numops < NUM_OPS)
                        node->ops[node->numops++] = (char)ch;
                else
                        xeprintf(5, "Not enough space for another optype!\n");
        }
}


static inline void
setup_node(struct expression *node)
{
        node->child    = xmalloc(MAXCHILD * sizeof *node->child);
        node->maxchild = MAXCHILD;
        node->nchild   = 0;
        node->numops   = 0;

        for (int i = 0; i < MAXCHILD; ++i) {
                node->child[i] = xmalloc(sizeof **node->child);
        }
}


static inline void
check_max(struct expression *node)
{
        if (node->nchild == node->maxchild) {
                node->child = xrealloc(node->child, ++node->maxchild *
                                                    sizeof *node->child);
                node->child[node->nchild] = xmalloc(sizeof **node->child);
        }
}
