#ifndef students_h
#define students_h

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


/* students.c */
char *program_name;


#endif /* students.h */
