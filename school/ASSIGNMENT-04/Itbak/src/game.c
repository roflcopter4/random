#include "tree.h"
/*#include <bsd/bsd.h>*/
#include <stdlib.h>
#include <string.h>

#define NUM_LISTS 1024
#define LIST_LEN 2

static void do_solve(struct Node *node);
static struct State **split(const uint8_t pile, uint * restrict nlists);


void
solve(struct Node *root)
{
        printf("Solving for %d tokens.\n", root->state->lst[0]);
        do_solve(root);
}


static void
do_solve(struct Node *node)
{
        for (int i = 0; i < node->state->len; ++i) {
                uint8_t pile = node->state->lst[i];

                if (pile <= 2)
                        continue;

                uint nlists = 0;
                struct State **state_list = split(pile, &nlists);

                for (uint A = 0; A < nlists; ++A) {
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
split(const uint8_t pile, uint * restrict nlists)
{
        uint8_t combinations[NUM_LISTS][LIST_LEN];
        uint8_t lengths[NUM_LISTS];
        uint range[pile], A, B, i, j;

        for (i = 0; i < pile; ++i)
                range[i] = i + 1;

        for (i = A = 0; i < pile; ++i) {
                uint val1 = range[i];
                for (j = B = 0; j < pile; ++j) {
                        uint val2 = range[j];
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
