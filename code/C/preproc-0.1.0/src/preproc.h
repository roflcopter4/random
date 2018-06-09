#ifndef SRC_PREPROC_H
#define SRC_PREPROC_H
/*===========================================================================*/
#ifdef __cplusplus
    extern "C" {
#endif
#ifdef _MSC_VER /* Microsoft sure likes to complain... */
#   pragma warning(disable : 4996)
#   define _CRT_SECURE_NO_WARNINGS
#   define _CRT_NONSTDC_NO_WARNINGS
#   define __attribute__(...)
#endif
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
#define STARTSIZE 8192

char *program_name;
int WIDTH;
bool IS_HEADER, NO_SKIP, ALL;


/*===========================================================================*/


#if (defined(_WIN64) || defined(_WIN32)) && !defined(__CYGWIN__)
#   define DOSISH
#   include <io.h>
#   include <Windows.h>
#   define strcasecmp  _stricmp
#   define strncasecmp _strnicmp
#   define S_ISREG(m) (((m) & S_IFMT) == S_IFREG)
#   define S_ISDIR(m) (((m) & S_IFMT) == S_IFDIR)
#   undef BUFSIZ
#   define BUFSIZ 8192
#   define PATHSEP '\\'
    extern char * basename(char *path);
#else
#   include <unistd.h>
#   define PATHSEP '/'
#endif

#define streq(SA, SB)    (strcmp((SA), (SB)) == 0)
#define strneq(SA, SB)   (strcmp((SA), (SB)) != 0)
#define strCeq(SA, SB)   (strcasecmp((SA), (SB)) == 0)
#define strCneq(SA, SB)  (strcasecmp((SA), (SB)) != 0)

#define modulo(A, B)     (((A) % (B) + (B)) % (B))
#define stringify(VAR_)  #VAR_
#define nputs(STR_)      fputs((STR_), stdout)

#ifdef HAVE_ERR
#   include <err.h>
#else
    void _warn(bool print_err, const char *fmt, ...) __attribute__((__format__(printf, 2, 3)));
#   define warn(...)       _warn(true, __VA_ARGS__)
#   define warnx(...)      _warn(false, __VA_ARGS__)
#   define err(EVAL, ...)  _warn(true, __VA_ARGS__), exit(EVAL)
#   define errx(EVAL, ...) _warn(false, __VA_ARGS__), exit(EVAL)
#endif
#define eputs(STR_)  fputs((STR_), stderr)
#define eprintf(...) fprintf(stderr, __VA_ARGS__)

#define xfree(PTR_) ((PTR_) ? free(PTR_), (PTR_) = NULL : NULL)

#define safe_stat(PATH, ST)                                      \
     do {                                                        \
             if ((stat((PATH), (ST)) != 0))                      \
                     err(1, "Failed to stat file '%s", (PATH));  \
     } while (0)


/*===========================================================================*/


/* utility.c */
#define xatoi(STR)      __xatoi((STR), false)
#define s_xatoi(STR)    __xatoi((STR), true)
extern int64_t __xatoi  (char *str, bool strict);
extern void *  xmalloc      (const size_t size)               
        __attribute__((__warn_unused_result__)) __attribute__((__malloc__));
extern void *  xcalloc      (const int num, const size_t size)
        __attribute__((__warn_unused_result__)) __attribute__((__malloc__));
extern void *  xrealloc     (void *ptr, const size_t size)
        __attribute__((__warn_unused_result__));
#ifdef HAVE_REALLOCARRAY
extern void *  xreallocarray(void *ptr, size_t num, size_t size)
        __attribute__((__warn_unused_result__));
#  define nmalloc(NUM_, SIZ_)        reallocarray(NULL, (NUM_), (SIZ_))
#  define nrealloc(PTR_, NUM_, SIZ_) xreallocarray((PTR_), (NUM_), (SIZ_))
#else
#  define nmalloc(NUM_, SIZ_)        malloc(((size_t)(NUM_)) * ((size_t)(SIZ_)))
#  define nrealloc(PTR_, NUM_, SIZ_) xrealloc((PTR_), ((size_t)(NUM_)) * ((size_t)(SIZ_)))
#endif


/* options.c */
extern bool handle_options(int argc, char **argv);


#ifdef __cplusplus
    }
#endif
/*===========================================================================*/
#endif /* preproc.h */
