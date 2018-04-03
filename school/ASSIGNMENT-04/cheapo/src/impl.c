#include "tree.h"
#include <string.h>
#include <stdlib.h>

#if 0
static struct Node *make_node();
#endif


struct Node *
init_tree(uint8_t val)
{
        struct Node *root = xmalloc(sizeof(*root));

        root->state.lst = xmalloc(1 * sizeof(*root->state.lst));
        root->state.lst[0] = val;
        root->state.len = 1;

        root->child    = xmalloc(max_child * sizeof(*root->child));
        root->maxchild = max_child;
        root->level    = 1;
        root->nchild   = 0;
        /*root->key      = root;*/
        /*root->parent   = root;*/

        return root;
}


void
destroy_tree(struct Node *node)
{
        for (int i = 0; i < node->nchild; ++i)
                destroy_tree(node->child[i]);

        free(node->child);
        free(node->state.lst);
        free(node);
}


struct Node *
new_node(struct Node *parent, struct State state)
{
        struct Node *node = xmalloc(sizeof(*node));

        /*node->key      = parent->key;*/
        /*node->parent   = parent;*/
        node->level    = !parent->level;
        node->nchild   = 0;
        node->state    = state;
        node->maxchild = max_child;
        node->child    = xmalloc(max_child * sizeof(*node->child));

        if (parent->nchild == parent->maxchild) {
                parent->maxchild += child_inc;
                parent->child = xrealloc(parent->child, parent->maxchild *
                                                        sizeof(*parent->child));
        }
        parent->child[parent->nchild++] = node;

        return node;
}


struct State
state_cpy(struct State *orig)
{
        struct State new;
        new.len = orig->len;
        new.lst = xmalloc(new.len * sizeof(*new.lst));
        memcpy(new.lst, orig->lst, new.len * sizeof(*new.lst));

        return new;
}


void
state_append(struct State *state, uint8_t *lst, uint16_t lst_len)
{
        uint16_t newlen = state->len + lst_len;
        state->lst = xrealloc(state->lst, newlen * sizeof(*state->lst));
        memcpy(&(state->lst[state->len]), lst, lst_len * sizeof(*state->lst));
        state->len = newlen;
}


#if 0
static struct Node *
make_node()
{
        struct Node *node = xmalloc(sizeof(*node));

        node->state.lst = NULL;
        node->state.len = 0;
        node->key       = NULL;
        node->parent    = NULL;
        node->level     = 0;
        node->nchild    = 0;
        node->maxchild  = max_child;
        node->child     = xmalloc(max_child * sizeof(struct Node *));

        return node;
}
#endif
