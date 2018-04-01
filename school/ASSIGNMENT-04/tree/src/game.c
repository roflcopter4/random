#include "tree.h"
#include <stdlib.h>
#include <string.h>

#define NUM_LISTS 1024
#define LIST_LEN 2

static struct Node *do_solve(struct Node *node);
static struct State *split(int pile, int *nlists);


void
solve(struct Node *root)
{
        printf("Solving for %d tokens.\n", root->state.lst[0]);

        do_solve(root);
}


static struct Node *
do_solve(struct Node *node)
{
        for (int i = 0; i < node->state.len; ++i) {
                int pile = node->state.lst[i];
                if (pile <= 2)
                        continue;

                int nlists;
                struct State *state_list = split(pile, &nlists);

                for (int listc = 0; listc < nlists; ++listc) {
                        struct State *cur = &state_list[listc];
                        struct State newstate = state_cpy(&node->state);

                        newstate.lst[i] = cur->lst[0];
                        state_append(&newstate, &cur->lst[1], cur->len - 1);
                        quick_sort(newstate.lst, newstate.len);

                        struct Node *child = new_node(node, newstate);
                        do_solve(child);

                        free(cur->lst);
                }
                free(state_list);
        }

        return NULL;
}


static struct State *
split(int pile, int *nlists)
{
        int comb[NUM_LISTS][LIST_LEN];
        int lengths[NUM_LISTS];

        int lst[pile];
        for (int i = 0; i < pile; ++i)
                lst[i] = i + 1;

        int listc, pilec;
        listc = 0;

        for (int i = 0; i < pile; ++i) {
                int val1 = lst[i];
                pilec = 0;
                for (int j = 0; j < pile; ++j) {
                        int val2 = lst[j];
                        if (val1 + val2 == pile) {
                                if (val1 >= val2)
                                        goto done;
                                comb[listc][pilec++] = val1;
                                comb[listc][pilec++] = val2;
                                break;
                        }
                }
                lengths[listc++] = pilec;
        }

done:
        *nlists = listc;
        struct State *state_list = xmalloc(*nlists * sizeof(*state_list));

        for (int i = 0; i < *nlists; ++i) {
                int *state = xmalloc(lengths[i] * sizeof(*state));
                memcpy(state, comb[i], lengths[i] * sizeof(*state));
                state_list[i].lst = state;
                state_list[i].len = lengths[i];
        }

        return state_list;
}
