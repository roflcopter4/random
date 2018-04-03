#include "tree.h"
#include <stdlib.h>
#include <string.h>

static struct State *check_cache(struct State *state);
static void add_cache(struct State *state);
static void destroy_nodes(struct Node *node);


struct Node *
init_tree(uint8_t val)
{
        struct Node *root = xmalloc(sizeof(*root));
        struct State *state = xmalloc(sizeof(*state));

        state->lst = xmalloc(1 * sizeof(*state->lst));
        state->lst[0] = val;
        state->len = 1;

        root->state    = state;
        root->child    = xmalloc(max_child * sizeof(*root->child));
        root->maxchild = max_child;
        root->level    = 1;
        root->nchild   = 0;

        cache.arr = xmalloc(cache_size * sizeof(*cache.arr));
        cache.arr[0] = state;
        cache.len  = 1;
        cache.max_size = cache_size;

        return root;
}

uint blah;

void
destroy_tree(struct Node *root)
{
        int i;
        for (i = 0; i < cache.len; ++i) {
                free(cache.arr[i]->lst);
                free(cache.arr[i]);
        }
        free(cache.arr);
        printf("There were %d records in cache.\n", i);

        destroy_nodes(root);

        printf("There were %d nodes.\n", blah);

        /*for (int i = 0; i < node->nchild; ++i)*/
                /*destroy_tree(node->child[i]);*/

        /*free(node->child);*/
        /*free(node->state->lst);*/
        /*free(node->state);*/
        /*free(node);*/
}


static void
destroy_nodes(struct Node *node)
{
        for (int i = 0; i < node->nchild; ++i)
                destroy_nodes(node->child[i]);

        free(node->child);
        /*free(node->state->lst);*/
        /*free(node->state);*/
        free(node);
        ++blah;
}


struct Node *
new_node(struct Node *parent, struct State *state)
{
        struct Node *node = xmalloc(sizeof(*node));

        struct State *cached;
        if ((cached = check_cache(state)) != NULL) {
                node->state = cached;
                free(state->lst);
                free(state);
        } else {
                node->state = state;
                add_cache(state);
                /*puts("This node is unique!\n");*/
        }

        node->level    = !parent->level;
        node->nchild   = 0;
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


struct State *
state_cpy(struct State *orig)
{
        struct State *new = xmalloc(sizeof(*new));
        new->len = orig->len;
        new->lst = xmalloc(new->len * sizeof(*new->lst));
        memcpy(new->lst, orig->lst, new->len * sizeof(*new->lst));

        return new;
}


void
state_append(struct State *dest, struct State *src, uint16_t start)
{
        uint16_t newlen = dest->len + (src->len - start);
        dest->lst = realloc(dest->lst, newlen * sizeof(*dest->lst));
        memcpy(dest->lst + dest->len,
               src->lst + start,
               (src->len - start) * sizeof(*dest->lst));
        dest->len = newlen;
}


static struct State *
check_cache(struct State *state)
{
        for (int i = 0; i < cache.len; ++i) {
                if (state->len == cache.arr[i]->len &&
                    memcmp(state->lst, cache.arr[i]->lst, state->len * sizeof(*state->lst)) == 0)
                        return cache.arr[i];
        }

        return NULL;
}


static void
add_cache(struct State *state)
{
        if (cache.len == cache.max_size) {
                cache.max_size += cache_inc;
                cache.arr = xrealloc(cache.arr, cache.max_size * sizeof(*cache.arr));
        }
        cache.arr[cache.len++] = state;
}
