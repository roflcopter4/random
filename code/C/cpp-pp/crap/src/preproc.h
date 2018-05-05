#ifndef SRC_PREPROC_H
#define SRC_PREPROC_H
/*===========================================================================*/
#ifdef HAVE_CONFIG_H
#  include "config.h"
#else  /* This just shuts up linters too lazy to include config.h */
#  define VERSION ""
#  define PACKAGE_STRING ""
#  define _GNU_SOURCE
#endif
#ifdef HAVE_STRLCPY
#  ifdef HAVE_BSD_BSD_H
#    include <bsd/bsd.h>
#  endif
#else
#  include "bsd_funcs.h"
#endif
/*===========================================================================*/


#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#define DEFAULT_WIDTH 2
#define STARTSIZE (2048 * sizeof(char))
#define INCSIZE   (256  * sizeof(char))

char *program_name;
int WIDTH;
bool IS_HEADER, NO_SKIP, ALL;


/*===========================================================================*/


#define streq(SA, SB)  (strcmp((SA), (SB)) == 0)
#define strneq(SA, SB) (strcmp((SA), (SB)) != 0)
#define modulo(A, B)   (((A) % (B) + (B)) % (B))
#define nputs(STR)     fputs((STR), stdout)
#define eprintf(...)   fprintf(stderr, __VA_ARGS__)

#define xfree(PTR) do { if ((PTR) != NULL) free(PTR); } while (0)

#define xeprintf(STATUS, ...)             \
    do {                                  \
            fprintf(stderr, __VA_ARGS__); \
            exit(STATUS);                 \
    } while (0)

#define xperror(FMT, ...)                              \
    do {                                               \
            char buf[BUFSIZ];                          \
            snprintf(buf, BUFSIZ, (FMT), __VA_ARGS__); \
            perror(buf);                               \
            exit(1);                                   \
    } while (0)


/*===========================================================================*/


/* utility.c */
#define xatoi(STR)      __xatoi((STR), false)
#define s_xatoi(STR)    __xatoi((STR), true)
int64_t __xatoi       (char *str, bool strict);

#if 0
#define my_getline(PTR) my_fgetline((PTR), stdin)
int     xasprintf     (char **restrict ptr, const char *restrict fmt, ...)__attribute__((format(printf,2,3)));
int     my_fgetline   (char **ptr, FILE *fp);
int     countlines    (char *filename);

void *  xmalloc       (size_t size) __attribute__((malloc));
void *  xcalloc       (int num, size_t size) __attribute__((malloc));
void *  xrealloc      (void *ptr, size_t size)__attribute__((malloc));

bool    file_is_reg   (const char *filename);
size_t  get_file_size (const char *filename);
#endif


/* options.c */
void handle_options(int argc, char **argv);


#endif /* preproc.h */
