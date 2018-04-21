#ifndef SRC_NEOTAGS_H
#define SRC_NEOTAGS_H
/*===========================================================================*/
#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif
#ifndef VERSION
#  define VERSION "0.1.0"
#endif
#ifndef NAME
#  define NAME "neotags_c"
#endif
#ifndef _GNU_SOURCE
#  define _GNU_SOURCE
#endif
#ifndef HAVE_STRLCPY
#  include "bsd_funcs.h"
#else
#  ifdef HAVE_BSD_BSD_H
#    include <bsd/bsd.h>
#  endif
#endif
/*===========================================================================*/


#include <stdbool.h>
#include <stdint.h>

#define streq(SA, SB)  (strcmp((SA), (SB)) == 0)
#define strneq(SA, SB) (strcmp((SA), (SB)) != 0)
#define modulo(A, B)   (((A) % (B) + (B)) % (B))
#define eprintf(...)   fprintf(stderr, __VA_ARGS__)
#define nputs(STR)     fputs((STR), stdout)


#define xeprintf(STATUS, ...)                 \
        do {                                  \
                fprintf(stderr, __VA_ARGS__); \
                exit(STATUS);                 \
        } while (0)


#define xatoi(STR)   __xatoi((STR), false)
#define s_xatoi(STR) __xatoi((STR), true)
int64_t __xatoi(const char *str, bool strict);

char *program_name;


#endif /* neotags.h */
