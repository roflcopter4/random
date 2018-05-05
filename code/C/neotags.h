#ifndef SRC_NEOTAGS_H
#define SRC_NEOTAGS_H
/*===========================================================================*/
#ifdef __cplusplus
   extern "C" {
#endif
#ifdef HAVE_CONFIG_H
#  include "config.h"
#else  /* This just shuts up linters too lazy to include config.h */
#  define VERSION ""
#  define PACKAGE_STRING ""
#  define _GNU_SOURCE
#endif
#ifndef HAVE_STRLCPY
#  if defined(HAVE_LIBBSD) && defined(HAVE_BSD_BSD_H)
#    include <bsd/bsd.h>
#  else
#    include "bsd_funcs.h"
#  endif
#endif
/*===========================================================================*/


#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#define LLTYPE char *

struct string {
        char *s;
        long long len;
};

struct strlist {
        char **s;
        uint32_t *slen;
        uint32_t num;
};

struct linked_list {
        struct Node *head;
        struct Node *tail;
        uint32_t size;
};

struct Node {
        LLTYPE data;
        struct Node *prev;
        struct Node *next;
};

enum ll_pop_type {
        DEL_ONLY,
        RET_ONLY,
        BOTH
};

char *program_name;
short try_strip_com;

/*===========================================================================*/


#if (defined(_WIN64) || defined(_WIN32)) && !defined(__CYGWIN__)
#  define Psize_t "%I64d"  /* Just have to assume size_t is size 8... */
#  include <io.h>
#else
#  define Psize_t "%zu"
#  include <unistd.h>
#endif

#ifdef _MSC_VER 
#  define strcasecmp  _stricmp
#  define strncasecmp _strnicmp
#  define S_ISREG(m) (((m) & S_IFMT) == S_IFREG)
#  define S_ISDIR(m) (((m) & S_IFMT) == S_IFDIR)
#  define __attribute__(...)
#  undef BUFSIZ
#  define BUFSIZ 8192
#endif

#define streq(SA, SB)    (strcmp((SA), (SB)) == 0)
#define strneq(SA, SB)   (strcmp((SA), (SB)) != 0)
#define strCeq(SA, SB)   (strcasecmp((SA), (SB)) == 0)
#define strCneq(SA, SB)  (strcasecmp((SA), (SB)) != 0)
#define modulo(A, B)     (((A) % (B) + (B)) % (B))
#define nputs(STR)       fputs((STR), stdout)
#define eprintf(...) fprintf(stderr, __VA_ARGS__)

#define xerr(STATUS, ...)                 \
    do {                                  \
            fprintf(stderr, __VA_ARGS__); \
            exit(STATUS);                 \
    } while (0)

#define xperror(...)                            \
    do {                                        \
            char buf[BUFSIZ];                   \
            snprintf(buf, BUFSIZ, __VA_ARGS__); \
            perror(buf);                        \
            exit(1);                            \
    } while (0)


/*===========================================================================*/


/* utility.c */
#define xatoi(STR)      __xatoi((STR), false)
#define s_xatoi(STR)    __xatoi((STR), true)
#define my_getline(PTR) my_fgetline((PTR), stdin)

long   __xatoi         (char *str, bool strict);
int    my_fgetline     (char **ptr, FILE *fp);
void   destroy_strlist (struct strlist *lst);
void * xmalloc         (const size_t size)                __attribute__((malloc));
void * xcalloc         (const int num, const size_t size) __attribute__((malloc));
void * xrealloc        (void *ptr, const size_t size)     __attribute__((malloc));
FILE * safe_fopen      (const char * const restrict filename, const char * const restrict mode);
void   dump_list       (char **list, FILE *fp);

/* struct strlist * get_all_lines(const char *filename); */
struct strlist * gz_get_all_lines(const char *filename);


/* linked_list.c */
#define ll_pop(LIST)        _ll_popat((LIST), -1, BOTH)
#define ll_dequeue(LIST)    _ll_popat((LIST), 0, BOTH)
#define ll_pop_at(LIST,IND) _ll_popat((LIST), (IND), BOTH)
#define ll_get(LIST,IND)    _ll_popat((LIST), (IND), RET_ONLY)
#define ll_remove(LIST,IND) _ll_popat((LIST), (IND), DEL_ONLY)

struct linked_list * new_list(void);

void   ll_add(struct linked_list *list, LLTYPE data);
void   ll_append(struct linked_list *list, LLTYPE data);
LLTYPE _ll_popat(struct linked_list *list, long index, enum ll_pop_type type);
bool   ll_find_str(struct linked_list *list, char *str);
void   destroy_list(struct linked_list *list);


/* auxil.c */
char * strip_comments(struct string *buffer, const char *lang);


#ifdef __cplusplus
   }
#endif
/*===========================================================================*/
#endif /* neotags.h */
