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
#if 0
#ifdef HAVE_STRLCPY
#  ifdef HAVE_BSD_BSD_H
#    include <bsd/bsd.h>
#  endif
#else
#  include "bsd_funcs.h"
#endif
#endif
/*===========================================================================*/


#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#define DEFAULT_WIDTH 2
#define STARTSIZE 2048
#define INCSIZE   256

char *program_name;
int WIDTH;
bool IS_HEADER, NO_SKIP, ALL;


/*===========================================================================*/


#define streq(SA, SB)  (strcmp((SA), (SB)) == 0)
#define strneq(SA, SB) (strcmp((SA), (SB)) != 0)
#define modulo(A, B)   (((A) % (B) + (B)) % (B))
#define nputs(STR)     fputs((STR), stdout)
#define eprintf(...)   fprintf(stderr, __VA_ARGS__)

#define xerr(STATUS, ...)                 \
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

#define safe_stat(PATH, ST)                                      \
     do {                                                        \
             if ((stat((PATH), (ST)) != 0))                      \
                     xperror("Failed to stat file '%s", (PATH)); \
     } while (0)


/*===========================================================================*/


/* utility.c */
#define xatoi(STR)      __xatoi((STR), false)
#define s_xatoi(STR)    __xatoi((STR), true)
extern int64_t __xatoi  (char *str, bool strict);
extern void *  xmalloc  (size_t size)           __attribute__((malloc));
extern void *  xcalloc  (int num, size_t size)  __attribute__((malloc));
extern void *  xrealloc (void *ptr, size_t size)__attribute__((malloc));


/* options.c */
extern bool handle_options(int argc, char **argv);


#endif /* preproc.h */
