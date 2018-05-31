#ifndef SRC_NEOTAGS_H
#define SRC_NEOTAGS_H
/*===========================================================================*/
#ifdef _MSC_VER /* Microsoft sure likes to complain... */
#   pragma warning(disable : 4996 4820)
#   define _CRT_SECURE_NO_WARNINGS
#   define _CRT_NONSTDC_NO_WARNINGS
#   define __attribute__(...)
#endif
#ifdef HAVE_CONFIG_H
#   include "config.h"
#else  /* This just shuts up linters too lazy to include config.h */
#   if defined(__GNUC__) || defined(__FreeBSD__)
#      define USE_PTHREADS
#      define HAVE_ERR
#   endif
#   define VERSION "1.1.0"
#   define PACKAGE_STRING "neotags 1.1.0"
#   define _GNU_SOURCE
#endif
#ifndef HAVE_STRLCPY
#   if defined(HAVE_LIBBSD) && defined(HAVE_BSD_BSD_H)
#      include <bsd/bsd.h>
#   else
#      include "bsd_funcs.h"
#   endif
#endif
#ifdef HAVE_MALLOPT
#   include <malloc.h>
#endif
/*===========================================================================*/

#include <cerrno>
#include <cstdint>
#include <cstdio>
#include <cstring>

#include <iostream>
#include <string>
#include <vector>

#define PROG_ID "C++"
#define INIT_TAGS 1024
#define TAGS_INC  512

/*===========================================================================*/

struct lldata {
        std::string s;
        char kind;
};

extern char *program_name;

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
#   define handle_progname(VAR_) VAR_
#else
    void _warn(bool print_err, const char *__restrict fmt, ...) \
            __attribute__((format(printf, 2, 3)));
#   define handle_progname(VAR_) basename(VAR_)
#   define warn(...)    _warn(true, __VA_ARGS__)
#   define warnx(...)   _warn(false, __VA_ARGS__)
#   define err(EVAL, ...)  _warn(true, __VA_ARGS__), exit(EVAL)
#   define errx(EVAL, ...) _warn(false, __VA_ARGS__), exit(EVAL)
//#      define warn(...)
//#      define warnx(...)
//#   define err(EVAL, ...) exit(EVAL)
//#   define errx(EVAL, ...) exit(EVAL)
#endif
#define eputs(STR_)  fputs((STR_), stderr)
#define eprintf(...) fprintf(stderr, __VA_ARGS__)


/*===========================================================================*/


/*
 * util.cpp
 */
#define xatoi(STR)      __xatoi((STR), false)
#define s_xatoi(STR)    __xatoi((STR), true)
#define my_getline(PTR) my_fgetline((PTR), stdin)
#define dump_list(LST_) __dump_list((LST_), stderr, #LST_)
#define free_all(...)   __free_all(__VA_ARGS__, nullptr)
#define dumpstr(STR_, FNAME_, FP_) __dump_string((STR_), (FNAME_), (FP_), (#STR_))

extern int64_t __xatoi     (char *str, bool strict);
extern void *  xmalloc     (size_t size)             __attribute__((malloc));
extern void *  xcalloc     (size_t num, size_t size) __attribute__((malloc));
extern void *  xrealloc    (void *ptr, size_t size);
extern FILE *  safe_fopen  (const char *__restrict filename,
                            const char *__restrict mode);
extern bool    file_is_reg (const char *filename);
extern void  __dump_list   (char **list, FILE *fp, const char *varname);
extern void  __dump_string (char *str, const char *filename, FILE *fp, const char *varname);
extern void  __free_all    (void *ptr, ...);
extern int   find_num_cpus ();


/* 
 * Else
 */
extern int getlines(std::vector<struct lldata> *tags,
                    const std::string *comptype, const std::string *filename);
extern char * strip_comments(struct lldata *buffer, const char *lang);


/*===========================================================================*/
#endif /* neotags.h */
