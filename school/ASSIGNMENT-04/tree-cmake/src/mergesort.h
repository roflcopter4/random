#ifndef mergesort_h
#define mergesort_h

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


/* utility.c */
char * shitty_fgetline(FILE *f);
int    countlines     (char *filename);
void   print_array    (char **array, int len);
void   free_s_array   (struct s_array *str_array);
void * xmalloc        (size_t size);
void * xcalloc        (int num, size_t size);
void * xrealloc       (void *ptr, size_t size);
void   shuffle        (char **array, size_t n);
int    xatoi          (char *str);


/* options.c */
void decode_switches(int argc, char **argv);
char optflags[32];
uint16_t verbose, loud, opt_flag_ind, use_intlist;
uint g_numitems, g_maxsize;


/* listgen.c */
void pretty_print(uint *intlist, uint size);
void print_intlist(uint *intlist, uint size);
uint * getlist(uint size, uint max);
uint * cpylist(uint *list, uint size);
void lazysort(uint *list, uint size);


/* oldmethods.c */
void bubble_sort   (uint *data, uint size);
void selection_sort(uint *data, uint size);
void recursive_ss  (uint *data, uint size);


/* impl.c */
void merge_sort(uint *data, uint size);


/* invertions.c */
/*long int __merge_sort(uint *data, uint size);*/


/* final.c */
int64 count_invertions(uint *data, uint size);


/* quicksort.c */
void quick_sort(uint *data, uint size);


#endif /* mergesort.h */
