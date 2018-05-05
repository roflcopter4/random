#ifndef sort_h
#define sort_h

/*===========================================================================*/
#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif
#ifndef VERSION
#  define VERSION "0.0.0"
#endif
#ifndef _GNU_SOURCE
#  define _GNU_SOURCE
#endif
/*===========================================================================*/
#include <sodium.h>
#include <stdbool.h>
#include <stdio.h>


struct s_array {
        char **arr;
        int *len;
        int num;
};


#define xasprintf(BUF, FMT, ...)                                          \
        do {                                                              \
                if (asprintf((BUF), (FMT), __VA_ARGS__) < 0) {            \
                        eprintf("Error allocating memory for asprintf."); \
                        exit(2);                                          \
                }                                                         \
        } while (0)

#define streq(SA, SB)  (strcmp((SA), (SB)) == 0)
#define strneq(SA, SB) (strcmp((SA), (SB)) != 0)
#define eprintf(...)   fprintf(stderr, __VA_ARGS__)
#define modulo(A, B)   (((A) % (B) + (B)) % (B))


/*#define INT_LIST                                                              \*/
        /*{ 300, 293, 286, 279, 272, 265, 258, 251, 244, 237, 230, 223, 216,    \*/
          /*209, 202, 195, 188, 181, 174, 167, 160, 153, 146, 139, 132, 125,    \*/
          /*118, 111, 104, 97, 90, 83, 76, 69, 62, 55, 48, 41, 34, 27, 20, 13,  \*/
          /*6, 0 }*/
/*#define IL_SIZE 44*/
/*#define INTLEN 3*/


/* utility.c */
char * shitty_fgetline(FILE *f);
int    countlines     (char *filename);
void   print_array    (char **array, int len);
void   free_s_array   (struct s_array *str_array);
void * die_malloc     (size_t size);
void * die_realloc    (void *ptr, size_t size);
void   shuffle        (char **array, size_t n);


/* options.c */
int decode_switches(int argc, char **argv);


/* sort.c */
char *program_name;
char optflags[128];
int  opt_flag_ind, numitems, maxsize, intlen;
bool verbose, loud;
void print_intlist(uint32_t *intlist, int size);


/* bubble.c */
void bubble_sort   (uint32_t *data, int size);
void selection_sort(uint32_t *data, int size);
void recursive_ss  (uint32_t *data, int size);


#endif /* sort.h */
