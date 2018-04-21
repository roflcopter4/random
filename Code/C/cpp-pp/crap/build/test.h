#ifndef SRC_PREPROC_H
#define SRC_PREPROC_H
/*===========================================================================*/
#ifdef HAVE_CONFIG_H
#include "config.h"
#else  /* This just shuts up linters too lazy to include config.h */
#define VERSION ""
#define PACKAGE_STRING ""
#define _GNU_SOURCE
#endif
#ifdef HAVE_STRLCPY
#ifdef HAVE_BSD_BSD_H
#include <bsd/bsd.h>
#endif
#else
#include "bsd_funcs.h"
#endif
/*===========================================================================*/


#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#define DEFAULT_WIDTH 2

char *program_name;
int WIDTH;


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
#define my_getline(PTR) my_fgetline((PTR), stdin)
int64_t __xatoi       (char *str, bool strict);
int     xasprintf     (char **restrict ptr, const char *restrict fmt, ...)__attribute__((format(printf,2,3)));

int     my_fgetline   (char **ptr, FILE *fp);
int     countlines    (char *filename);

void *  xmalloc       (size_t size) __attribute__((malloc));
void *  xcalloc       (int num, size_t size) __attribute__((malloc));
void *  xrealloc      (void *ptr, size_t size)__attribute__((malloc));

#if 0
bool    file_is_reg   (const char *filename);
size_t  get_file_size (const char *filename);
#endif


/* options.c */
void handle_options(int argc, char **argv);


/* quicksort.c */
#define QS_TYPE      uint8_t
#define QS_SIZE_TYPE uint16_t
void quick_sort(QS_TYPE *data, QS_SIZE_TYPE size);


#endif /* preproc.h */
