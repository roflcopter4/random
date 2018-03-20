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
#include <sodium.h>
#include <stdbool.h>
#include <stdio.h>


struct s_array {
        char **arr;
        int *len;
        int num;
};


struct student_t {
        char *name;
        int ID;
        int grade;
};


enum sort_type {
        sID,
        sname,
        sgrade
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


#define FILENAME "/home/bml/random/school/LAB/Lab09/info"

/* utility.c */
struct s_array parsefile (int argc, char **argv, int optind);
char * shitty_fgetline(FILE *f);
int    countlines     (char *filename);
void   print_array    (char **array, int len, char nl);
void   free_s_array   (struct s_array *str_array);
void * die_malloc     (size_t size);
void * die_realloc    (void *ptr, size_t size);
void   shuffle        (char **array, size_t n);
int    xatoi          (char *str);


/* options.c */
int decode_switches(int argc, char **argv);


/* students.c */
char *program_name;
int maxlen;


/* sortstudents.c */
void sort_students(struct student_t *data, int size, enum sort_type type);

/* methods.c */
/*void bubble_sort   (uint32_t *data, int size);*/
/*void selection_sort(uint32_t *data, int size);*/
/*void recursive_ss  (uint32_t *data, int size);*/


#endif /* students.h */
