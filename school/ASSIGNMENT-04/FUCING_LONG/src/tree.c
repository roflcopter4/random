#include "tree.h"
#include <stdlib.h>
#include <string.h>

static struct State *check_cache(struct State *state);
static void add_cache(struct State *state);
static void destroy_nodes(struct Node *node);

#define INDENT_SIZE BUFSIZ

struct Node *
init_tree(uint8_t val)
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
        root->depth    = 0;

        root->indent = malloc(INDENT_SIZE * sizeof(*root->indent));
        orig_indent  = root->indent;
        *root->indent++ = L' ';
        *root->indent = L'\0';

        cache.max_size = CACHE_SIZE;
        cache.arr      = malloc(cache.max_size * sizeof(*cache.arr));
        cache.arr[0]   = state;
        cache.len      = 1;

        return root;
}

uint32_t node_count;

void
destroy_tree(struct Node *root)
{
        uint32_t i;
        for (i = 0; i < cache.len; ++i) {
                free(cache.arr[i]->lst);
                free(cache.arr[i]);
        }
        free(cache.arr);
        printf("There were %u records in cache.\n", i);

        destroy_nodes(root);
        printf("There were %u nodes.\n", node_count);

        free(orig_indent);
}


static void
destroy_nodes(struct Node *node)
{
        for (uint32_t i = 0; i < node->nchild; ++i)
                destroy_nodes(node->child[i]);

        free(node->child);
        free(node);
        ++node_count;
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
        node->depth    = parent->depth + 1;
        node->indent   = parent->indent;

        if (parent->nchild == parent->maxchild) {
                parent->maxchild += CHILD_INC;
                parent->child = realloc(parent->child, parent->maxchild *
                                                       sizeof(*parent->child));
        }
        parent->child[parent->nchild++] = node;

        return node;
}


struct State *
state_cpy(struct State *orig)
{
        struct State *new = malloc(sizeof(*new));
        new->len = orig->len;
        new->lst = malloc(new->len * sizeof(*new->lst));
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
        for (int i = 0; i < cache.len; ++i)
                if (state->len == cache.arr[i]->len
                    && memcmp(state->lst,
                              cache.arr[i]->lst,
                              state->len * sizeof(*state->lst)) == 0
                    )
                        return cache.arr[i];
        return NULL;
}


static void
add_cache(struct State *state)
{
        if (cache.len == cache.max_size) {
                cache.max_size += CACHE_INC;
                cache.arr = realloc(cache.arr, cache.max_size * sizeof(*cache.arr));
        }
        cache.arr[cache.len++] = state;
}
