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
/*===========================================================================*/


#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>


enum node_lim {
        max_child = 0,
        child_inc = 1,
        /*cache_size = 128,*/
        /*cache_size = 1000,*/
        /*cache_inc = 16*/
};

struct State {
        uint8_t *lst;
        uint16_t len;
};

struct Node {
        struct Node **child;
        struct State *state;
        uint8_t nchild;
        uint8_t maxchild;
        bool level;
};


struct StateCache {
        /*struct State **arr;*/
        struct State *arr[500];
        uint16_t len;
        uint16_t max_size;
};


#if 0
struct s_array {
        char **arr;
        int *len;
        int num;
};
#endif


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


bool quiet, LEAF_ONLY;


/* utility.c */
#define   xatoi(STR)   __xatoi((STR), false)
#define   s_xatoi(STR) __xatoi((STR), true)
/*char    * my_fgetline  (FILE *f);*/
/*int       countlines   (char *filename);*/
/*void      print_array  (char **array, int len);*/
/*void      free_s_array (struct s_array *str_array);*/
void    * xmalloc      (size_t size);
void    * xcalloc      (int num, size_t size);
void    * xrealloc     (void *ptr, size_t size);
void      xfree        (void *ptr);
/*void      shuffle      (char **array, size_t n);*/
int64_t   __xatoi      (char *str, bool strict);
/*void      pretty_print (uint32_t *intlist, uint32_t size);*/


/* options.c */
void handle_options(int argc, char **argv);


/* main.c */
char *program_name;
struct StateCache cache;


/* impl.c */
struct Node  *new_node(struct Node *parent, struct State *state);
struct Node  *init_tree(uint8_t val);
struct State *state_cpy(struct State *orig);
void          destroy_tree(struct Node *node);
void          state_append(struct State *dest, struct State *src, uint16_t start);


/* game.c */
void solve(struct Node *root);


/* quicksort.c */
#define QSTYPE uint8_t
#define QS_SIZE_TYPE uint16_t
void quick_sort(QSTYPE *data, QS_SIZE_TYPE size);


/* display.c */
void display(struct Node *root);


#endif /* tree.h */
