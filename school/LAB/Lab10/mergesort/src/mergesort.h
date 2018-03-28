#ifndef mergesort_h
#define mergesort_h

/*===========================================================================*/
#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif
#ifndef VERSION
#  define VERSION "0.0.0"
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

typedef unsigned int uint32_t;
typedef short unsigned int uint16_t;

struct s_array {
        char **arr;
        int *len;
        int num;
};

struct ms_ptr_list {
        uint32_t **list;
        int increment;
        int max;
        int counter;
        /*int back_counter;*/
};


/*===========================================================================*/


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


/*===========================================================================*/


/*#define MAXSIZE 10000*/
#define MAXSIZE 4294967295
#define NUM     100
/*#define SPACE_INTS*/

/* mergesort.c */
char *program_name;


/* utility.c */
char * shitty_fgetline(FILE *f);
int    countlines     (char *filename);
void   print_array    (char **array, int len);
void   free_s_array   (struct s_array *str_array);
void * xmalloc        (size_t size);
void * xrealloc       (void *ptr, size_t size);
void   shuffle        (char **array, size_t n);
int    xatoi          (char *str);


/* options.c */
void decode_switches(int argc, char **argv);
char optflags[32];
uint16_t verbose, loud, opt_flag_ind;
uint32_t numitems, maxsize;


/* listgen.c */
#define lazy_printlist(LST) print_intlist((LST), numitems)
void print_intlist(uint32_t *intlist, int size);
uint32_t * getlist();
uint32_t * cpylist(uint32_t *list);
void lazysort(uint32_t *list, int size);


/* oldmethods.c */
void bubble_sort   (uint32_t *data, int size);
void selection_sort(uint32_t *data, int size);
void recursive_ss  (uint32_t *data, int size);


/* impl.c */
uint32_t * merge_sort(uint32_t *data, int size);


/* invertions.c */
uint32_t * __merge_sort(uint32_t *data, int size);


#endif /* mergesort.h */
