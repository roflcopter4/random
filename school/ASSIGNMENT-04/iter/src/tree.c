#include "tree.h"
#include <stdlib.h>
#include <string.h>

static struct State *check_cache(const struct State *state);
static void add_cache(struct State *state);

uint32_t node_count;


struct Node *
init_tree(const uint8_t val)
{
        struct Node *root   = malloc(sizeof(*root));
        struct State *state = malloc(sizeof(*state));

        state->lst    = malloc(sizeof(*state->lst));
        state->lst[0] = val;
        state->len    = 1;

        root->child    = malloc(MAX_CHILD * sizeof(*root->child));
        root->state    = state;
        root->level    = true;
        root->nchild   = 0;
        root->maxchild = MAX_CHILD;

        /*
         * This string won't ever be terribly big, BUFSIZ should be more than
         * overkill to hold it all.
         */
        root->indent = malloc(BUFSIZ * sizeof(*root->indent));
        orig_indent  = root->indent;
        *root->indent++ = ' ';
        *root->indent   = '\0';

        cache.max_size = CACHE_SIZE;
        cache.arr      = malloc(cache.max_size * sizeof(*cache.arr));
        cache.arr[0]   = state;
        cache.len      = 1;

        return root;
}


void
destroy_tree(struct Node *root)
{
        uint32_t i;
        for (i = 0; i < cache.len; ++i) {
                free(cache.arr[i]->lst);
                free(cache.arr[i]);
        }
        free(cache.arr);
        eprintf("There were %u records in cache.\n", i);

        destroy_node(root);
        eprintf("There were %u nodes.\n", node_count);

        free(orig_indent);
}


void
destroy_node(struct Node *node)
{
        for (uint32_t i = 0; i < node->nchild; ++i)
                destroy_node(node->child[i]);

        free(node->child);
        free(node);
        ++node_count;  /* eww, global shenanigans */
}


struct Node *
new_node(struct Node *parent, struct State *state)
{
        struct Node *node = malloc(sizeof(*node));
        struct State *cached;

        if ((cached = check_cache(state)) != NULL) {
                node->state = cached;
                free(state->lst);
                free(state);
        } else {
                node->state = state;
                add_cache(state);
        }

        node->child    = malloc(MAX_CHILD * sizeof(*node->child));
        node->level    = !parent->level;
        node->nchild   = 0;
        node->maxchild = MAX_CHILD;
        node->indent   = parent->indent + INDENT_SIZE;

        if (parent->nchild == parent->maxchild) {
                parent->maxchild += CHILD_INC;
                parent->child = realloc(parent->child, parent->maxchild *
                                                       sizeof(*parent->child));
        }
        parent->child[parent->nchild++] = node;

        return node;
}


struct State *
state_cpy(const struct State * const orig)
{
        struct State *new = malloc(sizeof(*new));
        new->len = orig->len;
        new->lst = malloc(new->len * sizeof(*new->lst));
        memcpy(new->lst, orig->lst, new->len * sizeof(*new->lst));

        return new;
}


void
state_append(struct State *dest,
             const struct State * const src,
             const uint16_t start)
{
        uint16_t newlen = dest->len + (src->len - start);
        dest->lst = realloc(dest->lst, newlen * sizeof(*dest->lst));
        memcpy(dest->lst + dest->len,
               src->lst + start,
               (src->len - start) * sizeof(*dest->lst));
        dest->len = newlen;
}


static struct State *
check_cache(const struct State * const state)
{
        for (int i = 0; i < cache.len; ++i) {
                if (state->len == cache.arr[i]->len &&
                    memcmp(state->lst,
                           cache.arr[i]->lst,
                           state->len * sizeof(*state->lst)
                          ) == 0
                    )
                        return cache.arr[i];
        }

        return NULL;
}


static void
add_cache(struct State * const state)
{
        if (cache.len == cache.max_size) {
                cache.max_size += CACHE_INC;
                cache.arr = realloc(cache.arr, cache.max_size * sizeof(*cache.arr));
        }
        cache.arr[cache.len++] = state;
}
