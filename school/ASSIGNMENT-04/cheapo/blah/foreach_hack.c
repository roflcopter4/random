#include "tree.h"
#include "foreach_hack.h"
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

#define NUM_LISTS 1024
#define LIST_LEN 2

static void do_solve(struct Node *node);
static struct State *split(uint8_t pile, int *nlists);
/*static inline void * dummy_wrapper(void *data);*/


static void
do_solve(struct Node *node)
{
        foreach_lC (uint8_t *pile, i, node->state.lst, node->state.len) {
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
split(uint8_t pile, int *nlists)
{
        uint8_t combinations[NUM_LISTS][LIST_LEN];
        uint8_t lengths[NUM_LISTS];
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
                uint8_t *state = xmalloc(lengths[i] * sizeof(*state));
                memcpy(state, combinations[i], lengths[i] * sizeof(*state));
                state_list[i].lst = state;
                state_list[i].len = lengths[i];
        }

        return state_list;
}
