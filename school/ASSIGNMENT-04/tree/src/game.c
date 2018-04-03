#include "foreach_hack.h"
#include "tree.h"
#include <stdlib.h>
#include <string.h>

#define NUM_LISTS 1024
#define LIST_LEN 2

static void do_solve(struct Node *node);
static struct State *split(uint16_t pile, int *nlists);


void
solve(struct Node *root)
{
        printf("Solving for %d tokens.\n", root->state.lst[0]);

        do_solve(root);
}


#if 0
static void
do_solve(struct Node *node)
{
        foreach_lC (uint16_t *pile, i, node->state.lst, node->state.len) {
                if (*pile <= 2)
                        continue;

                int nlists;
                struct State *state_list = split(*pile, &nlists);

                foreach_l (struct State *cur, state_list, nlists) {
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
}


static struct State *
split(uint16_t pile, int *nlists)
{
        uint16_t combinations[NUM_LISTS][LIST_LEN];
        uint16_t lengths[NUM_LISTS];
        int range[pile];

        for (int i = 0; i < pile; ++i)
                range[i] = i + 1;

        int listc = 0;

        foreach_f (int *val1, range) {
                int pilec = 0;
                foreach_f (int *val2, range)
                        if (*val1 + *val2 == pile) {
                                if (*val1 >= *val2)
                                        goto done;
                                combinations[listc][pilec++] = *val1;
                                combinations[listc][pilec++] = *val2;
                                break;
                        }
                lengths[listc++] = pilec;
        }

done:
        *nlists = listc;
        struct State *state_list = xmalloc(*nlists * sizeof(*state_list));

        for (int i = 0; i < *nlists; ++i) {
                uint16_t *state = xmalloc(lengths[i] * sizeof(*state));
                memcpy(state, combinations[i], lengths[i] * sizeof(*state));
                state_list[i].lst = state;
                state_list[i].len = lengths[i];
        }

        return state_list;
}
#endif


static void
do_solve(struct Node *node)
{
        for (int i = 0; i < node->state.len; ++i) {
                uint16_t pile = node->state.lst[i];
                if (pile <= 2)
                        continue;

                int nlists;
                struct State *state_list = split(pile, &nlists);

                for (int listc = 0; listc < nlists; ++listc) {
                        struct State *cur     = state_list + listc;
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
}


static struct State *
split(uint16_t pile, int *nlists)
{
        uint16_t combinations[NUM_LISTS][LIST_LEN];
        uint16_t lengths[NUM_LISTS];
        int range[pile];
        int listc, pilec, i, j;

        for (i = 0; i < pile; ++i)
                range[i] = i + 1;

        listc = 0;
        for (i = 0; i < pile; ++i) {
                int val1 = range[i];
                pilec = 0;
                for (j = 0; j < pile; ++j) {
                        int val2 = range[j];

                        if (val1 + val2 == pile) {
                                if (val1 >= val2)
                                        goto done;
                                combinations[listc][pilec++] = val1;
                                combinations[listc][pilec++] = val2;
                                break;
                        }
                }
                lengths[listc++] = pilec;
        }
done:
        *nlists = listc;
        struct State *state_list = xmalloc(*nlists * sizeof(*state_list));

        for (i = 0; i < *nlists; ++i) {
                uint16_t *state = xmalloc(lengths[i] * sizeof(*state));
                memcpy(state, combinations[i], lengths[i] * sizeof(*state));
                state_list[i].lst = state;
                state_list[i].len = lengths[i];
        }

        return state_list;
}
