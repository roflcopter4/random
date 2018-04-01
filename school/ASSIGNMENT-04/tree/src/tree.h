#ifndef tree_h
#define tree_h
/*===========================================================================*/
#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif
#ifndef VERSION
#  define VERSION "0.1.0"
#endif
#ifndef NAME
#  define NAME "mergesort"
#endif
#ifndef _GNU_SOURCE
#  define _GNU_SOURCE
#endif
/*===========================================================================*/


#include <stdbool.h>
#include <stdio.h>


typedef short unsigned int uint16_t;
typedef unsigned int uint32_t;
typedef unsigned int uint;
typedef long int int64;


/*#define nstate_lists   2048*/
/*#define state_list_len 2*/
/*#define max_child      16*/
/*#define child_inc      8*/

enum node_lim {
        nstate_lists = 1024,
        max_child = 6,
        child_inc = 3
};

struct State {
        int *lst;
        int len;
};

struct Node {
        /*int **state;*/
        /*int nlists;*/
        /*int *state;*/
        /*int state_len;*/

        struct State state;

        /*char level[level_size];*/
        uint16_t level;

        struct Node *key;
        struct Node *parent;
        struct Node **child;
        int nchild;
        int maxchild;

        int depth;
};


struct s_array {
        char **arr;
        int *len;
        int num;
};


/*===========================================================================*/


#define streq(SA, SB)  (strcmp((SA), (SB)) == 0)
#define strneq(SA, SB) (strcmp((SA), (SB)) != 0)
#define modulo(A, B)   (((A) % (B) + (B)) % (B))
#define eprintf(...)   fprintf(stderr, __VA_ARGS__)

#define xeprintf(STATUS, ...)                 \
        do {                                  \
                fprintf(stderr, __VA_ARGS__); \
                exit(STATUS);                 \
        } while (0)

#define xasprintf(BUF, FMT, ...)                                                \
        do {                                                                    \
                if (asprintf((BUF), (FMT), __VA_ARGS__) < 0)                    \
                        xeprintf(2, "Error allocating memory for asprintf.\n"); \
        } while (0)


/*===========================================================================*/


/* mergesort.c */
char *program_name;
bool verbose;


/* utility.c */
char * shitty_fgetline(FILE *f);
int    countlines     (char *filename);
void   print_array    (char **array, int len);
void   free_s_array   (struct s_array *str_array);
void * xmalloc        (size_t size);
void * xcalloc        (int num, size_t size);
void * xrealloc       (void *ptr, size_t size);
void   xfree          (void *ptr);
void   shuffle        (char **array, size_t n);
int    xatoi          (char *str);


/* options.c */
void decode_switches(int argc, char **argv);


/* main.c */
char *program_name;


/* impl.c */
struct Node * new_node(struct Node *parent, struct State state);
struct Node *init_tree(int val);
struct State state_cpy(struct State *orig);
void destroy_tree(struct Node *node);
void state_append(struct State *state, int *lst, int lst_len);


/* game.c */
void solve(struct Node *root);


/* quicksort.c */
void quick_sort(int *data, int size);


/* display.c */
void display(struct Node *root);


#endif /* tree.h */
