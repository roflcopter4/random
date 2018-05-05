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


#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

typedef int64_t int64;
typedef uint32_t uint;

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


/*#define SPACE_INTS*/


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
int64  xatoi          (char *str);


/* options.c */
void decode_switches(int argc, char **argv);
char optflags[32];
uint16_t verbose, loud, opt_flag_ind, use_intlist;
int64 g_numitems, g_maxsize;
bool DO_NOTHING;


/* listgen.c */
void pretty_print(uint *intlist, int64 size);
void print_intlist(uint *intlist, int64 size);
uint * getlist(int64 size, int64 max);
uint * cpylist(uint *list, int64 size);
void lazysort(uint *list, int64 size);


/* oldmethods.c */
void bubble_sort   (uint *data, int64 size);
void selection_sort(uint *data, int64 size);
void recursive_ss  (uint *data, int64 size);


/* impl.c */
void merge_sort(uint *data, int64 size);


/* invertions.c */
long int __merge_sort(uint *data, int64 size);


/* final.c */
long int count_invertions(uint *data, int64 size);


/* quicksort.c */
#define QSTYPE uint
#define QS_SIZE_TYPE int64
void quick_sort(QSTYPE *data, QS_SIZE_TYPE size);


#endif /* mergesort.h */
