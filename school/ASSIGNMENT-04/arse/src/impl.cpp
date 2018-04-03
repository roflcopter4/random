#include "arse.h"
#include <cstdlib>
#include <cstring>


struct Node *
init_tree(uint8_t val)
{
        auto root = new struct Node;

        root->state.lst    = static_cast<uint8_t *>( xmalloc(sizeof(*root->state.lst)) );
        root->state.lst[0] = val;
        root->state.len    = 1;

        root->child    = static_cast<struct Node **>( xmalloc(max_child * sizeof(*root->child)) );
        root->maxchild = max_child;
        root->level    = 1;
        root->nchild   = 0;
        /*root->key    = root;*/
        /*root->parent = root;*/

        return root;
}


void
destroy_tree(struct Node *node)
{
        for (int i = 0; i < node->nchild; ++i)
                destroy_tree(node->child[i]);

        free(node->child);
        free(node->state.lst);
        delete node;
}


struct Node *
new_node(struct Node *parent, struct State state)
{
        auto *node = new struct Node;

        /*node->key      = parent->key;*/
        /*node->parent   = parent;*/
        node->level    = !parent->level;
        node->nchild   = 0;
        node->state    = state;
        node->maxchild = max_child;
        node->child    = static_cast<struct Node **>( xmalloc(max_child * sizeof(*node->child)) );

        if (parent->nchild == parent->maxchild) {
                parent->maxchild += child_inc;
                parent->child = static_cast<struct Node **>( xrealloc(parent->child, parent->maxchild *
                                                                                     sizeof(*parent->child)));
        }
        parent->child[parent->nchild++] = node;

        return node;
}


struct State
state_cpy(struct State *orig)
{
        struct State copy;
        copy.len = orig->len;
        copy.lst = static_cast<uint8_t *>( xmalloc(copy.len * sizeof(*copy.lst)) );
        memcpy(copy.lst, orig->lst, copy.len * sizeof(*copy.lst));

        return copy;
}


void
state_append(struct State *state, uint8_t *lst, uint8_t lst_len)
{
        uint8_t newlen = state->len + lst_len;
        state->lst = static_cast<uint8_t *>( xrealloc(state->lst, newlen * sizeof(*state->lst)) );
        memcpy(&(state->lst[state->len]), lst, lst_len * sizeof(*state->lst));
        state->len = newlen;
}

// vim: set sts=8 sw=8
