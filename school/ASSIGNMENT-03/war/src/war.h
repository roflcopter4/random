#ifndef war_h
#define war_h

/*===========================================================================*/
#ifndef VERSION
#  define VERSION "1.0.0"
#endif
#ifndef _GNU_SOURCE
#  define _GNU_SOURCE
#endif
/*===========================================================================*/
#include <stdbool.h>

/* Misc */

/* Because the first thing we do is assert that there must be 52 strings, and
 * because the strings must be of size 2 (therefore 3 with the null char),
 * defining an array literally with these constants should be safe. */
#define D_SIZE 52
#define C_SIZE 2

struct s_array {
        char **arr;
        int *len;
        int num;
};

struct table_t {
        int count;
        bool face_up[D_SIZE];
        char cards[D_SIZE][C_SIZE + 1];
};


#define streq(SA, SB)  (strcmp((SA), (SB)) == 0)
#define strneq(SA, SB) (strcmp((SA), (SB)) != 0)
#define eprintf(...)   fprintf(stderr, __VA_ARGS__)
#define modulo(A, B)   (((A) % (B) + (B)) % (B))


/* options.c */
int decode_switches(int argc, char **argv, int *numcards);


/* utility.c */
char * shitty_fgetline(FILE *f);
int    countlines     (char *filename);
void   print_array    (char **array, int len);
void   free_s_array   (struct s_array *str_array);
void * die_malloc     (size_t size);
void * die_realloc    (void *ptr, size_t size);
void   shuffle        (char **array, size_t n);


/* validate.c */
void validate_data(struct s_array *data);


/* war.c */
char *program_name;


#endif /* war.h */
