#include "tree.h"
/*#include <bsd/bsd.h>*/
#include <stdlib.h>
#include <string.h>

#define NUM_LISTS 1024
#define LIST_LEN 2
#define MAX_THREADS 12

static void do_solve(struct Node *node);
static struct State **split(uint8_t pile, int *nlists);

#ifdef USE_THREADS
#  include <pthread.h>

   static inline void *wrapper(void *data);

   struct Node *ROOT;
   pthread_t tid[MAX_THREADS];
#endif


void
solve(struct Node *root)
{
        printf("Solving for %d tokens.\n", root->state->lst[0]);
#ifdef USE_THREADS
        ROOT = root;
#endif
        do_solve(root);
}


#ifdef USE_THREADS
static inline void *
wrapper(void *data)
{
        do_solve((struct Node *)data);
        pthread_exit(NULL);
}
#endif


static void
do_solve(struct Node *node)
{
        int nlists = 0;

        for (int i = 0; i < node->state->len; ++i) {
                uint8_t pile = node->state->lst[i];

                if (pile <= 2)
                        continue;

                struct State **state_list = split(pile, &nlists);

                for (int listc = 0; listc < nlists; ++listc) {
                        struct State *cur      = state_list[listc];
                        struct State *newstate = state_cpy(node->state);
                        newstate->lst[i]       = cur->lst[0];

                        state_append(newstate, cur, 1);
                        quick_sort(newstate->lst, newstate->len);

                        struct Node *child = new_node(node, newstate);

#ifdef USE_THREADS
                        if (node == ROOT && listc < MAX_THREADS)
                                pthread_create(tid + listc, 0, wrapper, child);
                        else
                                do_solve(child);
#else
                        do_solve(child);
#endif

                        free(cur->lst);
                        free(cur);
                }
                free(state_list);
        }

#ifdef USE_THREADS
        if (node == ROOT)
                for (int i = 0; i < nlists && i < MAX_THREADS; ++i)
                        pthread_join(tid[i], NULL);
#endif
}


static struct State **
split(uint8_t pile, int *nlists)
{
        uint8_t combinations[NUM_LISTS][LIST_LEN];
        uint8_t lengths[NUM_LISTS];
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
        struct State **state_list = malloc(*nlists * sizeof(*state_list));

        for (i = 0; i < *nlists; ++i) {
                state_list[i]  = malloc(sizeof(**state_list));
                uint8_t *combo = malloc(lengths[i] * sizeof(*combo));
                memcpy(combo, combinations[i], lengths[i] * sizeof(*combo));
                state_list[i]->lst = combo;
                state_list[i]->len = lengths[i];
        }

        return state_list;
}
