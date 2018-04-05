#include "arse.h"
#include <cstdlib>
#include <cstring>
#include <pthread.h>

#include <iostream>
#include <memory>
#include <string>
#include <vector>

#define NUM_LISTS 1024
#define LIST_LEN 2
#define MAX_THREADS 12

static void do_solve(struct Node *node);
static struct State *split(uint8_t pile, int *nlists);
static inline void * wrapper(void *data);

struct Node *ROOT;
pthread_t tid[MAX_THREADS];


void
solve(struct Node *root)
{
        std::cout << "Solving for " << root->state.lst[0] << " tokens.\n";
        ROOT = root;
        do_solve(root);
}


static inline void *
wrapper(void *data)
{
        do_solve(static_cast<struct Node *>( data ));
        pthread_exit(nullptr);
}


static void
do_solve(struct Node *node)
{
        int nlists = 0;

        for (int i = 0; i < node->state.len; ++i) {
                uint8_t pile = node->state.lst[i];

                if (pile <= 2)
                        continue;

                struct State *state_list = split(pile, &nlists);

                for (int listc = 0; listc < nlists; ++listc) {
                        struct State *cur     = state_list + listc;
                        struct State newstate = state_cpy(&node->state);
                        struct Node *child;

                        newstate.lst[i] = cur->lst[0];
                        state_append(&newstate, &cur->lst[1], cur->len - 1);
                        quick_sort(newstate.lst, newstate.len);
                        child = new_node(node, newstate);

                        if (node == ROOT && listc < MAX_THREADS)
                                pthread_create(tid + listc, 0, wrapper, child);
                        else
                                do_solve(child);

                        delete[] (state_list + listc)->lst;
                }
                delete[] state_list;
        }

        if (node == ROOT)
                for (int i = 0; i < nlists && i < MAX_THREADS; ++i)
                        pthread_join(tid[i], NULL);
}


static struct State *
split(uint8_t pile, int *nlists)
{
        uint8_t combinations[NUM_LISTS][LIST_LEN];
        uint8_t lengths[NUM_LISTS];
        auto range = new int[pile];
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
        //auto state_list = std::vector<struct State>;
        //std::vector<struct State>;
        auto state_list = new struct State [*nlists];

        for (i = 0; i < *nlists; ++i) {
                //auto state = new uint8_t [lengths[i]];
                //std::unique_ptr<uint8_t *> state { new  }
                auto state = std::make_unique<uint8_t>(lengths[i]);
                memcpy(state, combinations[i], lengths[i] * sizeof(*state));
                state_list[i].lst = state;
                state_list[i].len = lengths[i];
        }

        delete[] range;
        return state_list;
}
