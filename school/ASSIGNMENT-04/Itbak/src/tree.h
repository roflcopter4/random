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
#  define NAME "tree"
#endif
#ifndef _GNU_SOURCE
#  define _GNU_SOURCE
#endif
#ifndef HAVE_STRLCPY
#  include "bsd_funcs.h"
#else
#  ifdef HAVE_BSD_BSD_H
#    include <bsd/bsd.h>
#  endif
#endif
/*===========================================================================*/


#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#define MAX_CHILD 0
#define CHILD_INC 1
#define CACHE_SIZE 128
#define CACHE_INC 16

typedef unsigned int uint;

struct State {
        uint8_t *lst;
        uint16_t len;
};

struct Node {
        struct Node **child;
        struct State *state;
        uint16_t depth;
        uint8_t nchild;
        uint8_t maxchild;
        char *indent;
        /* uint i; */
        bool level;
};

struct StateCache {
        struct State **arr;
        uint16_t len;
        uint16_t max_size;
};

char *orig_indent;


/*===========================================================================*/


#define streq(SA, SB)  (strcmp((SA), (SB)) == 0)
#define strneq(SA, SB) (strcmp((SA), (SB)) != 0)
#define modulo(A, B)   (((A) % (B) + (B)) % (B))
#define eprintf(...)   fprintf(stderr, __VA_ARGS__)
#define nputs(STR)     fputs((STR), stdout)

#define xeprintf(STATUS, ...)             \
    do {                                  \
            fprintf(stderr, __VA_ARGS__); \
            exit(STATUS);                 \
    } while (0)

#define xasprintf(BUF, FMT, ...)                                        \
    do {                                                                \
            if (asprintf((BUF), (FMT), __VA_ARGS__) < 0)                \
                xeprintf(2, "Error allocating memory for asprintf.\n"); \
    } while (0)


/*===========================================================================*/


/* utility.c */
#define   xatoi(STR)   __xatoi((STR), false)
#define   s_xatoi(STR) __xatoi((STR), true)
int64_t   __xatoi      (char *str, bool strict);
void    * xmalloc      (size_t size);
void    * xcalloc      (int num, size_t size);
void    * xrealloc     (void *ptr, size_t size);
void      xfree        (void *ptr);


/* options.c */
bool quiet, LEAF_ONLY;
void handle_options(int argc, char **argv);


/* main.c */
char *program_name;
struct StateCache cache;


/* tree.c */
struct Node  * new_node     (struct Node *parent, struct State *state);
struct Node  * init_tree    (uint8_t val);
struct State * state_cpy    (struct State *orig);
void           destroy_tree (struct Node *node);
void           state_append (struct State *dest, struct State *src, uint16_t start);
void destroy_nodes(struct Node *node);


/* game.c */
void solve(struct Node *root);


/* quicksort.c */
#define QSTYPE uint8_t
#define QS_SIZE_TYPE uint16_t
void quick_sort(QSTYPE *data, QS_SIZE_TYPE size);


/* display.c */
void display(struct Node *root);
void print_node(struct Node *node, bool last);


/* iter.c */
void iter_solve(struct Node *root);


#endif /* tree.h */
