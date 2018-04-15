#include "tree.h"
#include <stdlib.h>
#include <string.h>

#define NUM_LISTS 1024
#define LIST_LEN 2

static void do_solve(struct Node *node);
static void fuck(struct Node *node);
static struct State **split(uint8_t const pile, uint16_t * const nlists);

struct Node *ROOT;


void
iter_solve(struct Node *root)
{
        printf("Solving for %d tokens.\n", root->state->lst[0]);
        /* do_solve(root); */
        print_node(root, 0);
        ROOT = root;
        fuck(root);
}


static void
do_solve(struct Node *node)
{
        /* print_node(node); */

        for (int i = 0; i < node->state->len; ++i) {
                uint8_t pile = node->state->lst[i];

                if (pile <= 2)
                        continue;

                uint16_t nlists = 0;
                struct State **state_list = split(pile, &nlists);

                for (uint16_t A = 0; A < nlists; ++A) {
                        struct State *newstate = state_cpy(node->state);
                        newstate->lst[i] = state_list[A]->lst[0];

                        state_append(newstate, state_list[A], 1);
                        quick_sort(newstate->lst, newstate->len);

                        struct Node *child = new_node(node, newstate);
                        do_solve(child);

                        free(state_list[A]->lst);
                        free(state_list[A]);
                }

                free(state_list);
        }
}


static struct State **
split(uint8_t const pile, uint16_t * const nlists)
{
        uint8_t combinations[NUM_LISTS][LIST_LEN];
        uint8_t lengths[NUM_LISTS];
        uint16_t range[pile], A, B, i, j;

        for (i = 0; i < pile; ++i)
                range[i] = i + 1;

        for (i = A = 0; i < pile; ++i) {
                uint16_t val1 = range[i];
                for (j = B = 0; j < pile; ++j) {
                        uint16_t val2 = range[j];
                        if (val1 + val2 == pile) {
                                if (val1 >= val2)
                                        goto done;
                                combinations[A][B++] = val1;
                                combinations[A][B++] = val2;
                                break;
                        }
                }
                lengths[A++] = B;
        }

done:
        *nlists = A;
        struct State **state_list = malloc(*nlists * sizeof(*state_list));

        for (i = 0; i < *nlists; ++i) {
                uint8_t *combo = malloc(lengths[i] * sizeof(*combo));
                state_list[i]  = malloc(sizeof(**state_list));

                memcpy(combo,
                       combinations[i],
                       lengths[i] * sizeof(*combo));

                state_list[i]->lst = combo;
                state_list[i]->len = lengths[i];
        }

        return state_list;
}

static void
/* fuck(struct Node *root) */
fuck(struct Node *node)
{
        /* struct Node *node = root; */
        bool printed = (node == ROOT);

        for (int i = 0; i < node->state->len; ++i) {
                uint8_t pile = node->state->lst[i];

                if (pile <= 2)
                        continue;

                uint16_t nlists = 0;
                struct State **state_list = split(pile, &nlists);

                if (!printed) {
                        print_node(node, nlists);
                        printed = true;
                }

                for (uint16_t A = 0; A < nlists; ++A) {
                        struct State *newstate = state_cpy(node->state);
                        newstate->lst[i] = state_list[A]->lst[0];

                        state_append(newstate, state_list[A], 1);
                        quick_sort(newstate->lst, newstate->len);

                        /* struct Node *child = new_node(node, newstate); */
                        new_node(node, newstate);

                        free(state_list[A]->lst);
                        free(state_list[A]);
                }

                free(state_list);
        }
        
        if (!printed)
                print_node(node, node->nchild);

        for (uint16_t i = 0; i < node->nchild; ++i)
                fuck(node->child[i]);
}
