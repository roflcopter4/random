#ifndef tree_h
#define tree_h
/*===========================================================================*/
#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif
#ifndef VERSION
#  define VERSION "0.1.0"
#endif
#ifndef NAME
#  define NAME "tree"
#endif
#ifndef _GNU_SOURCE
#  define _GNU_SOURCE
#endif
/*===========================================================================*/


#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#define NUM_OPS 64

typedef uint32_t uint;

struct str_list {
        char **data;
        uint32_t *sizes;
        uint32_t num;
};


enum Operators {
        None     = 0,
        Add      = '+',
        Subtract = '-',
        Multiply = '*',
        Divide   = '/',
        Modulo   = '%',
        Power    = '^'
};


struct Child {
        union data_u {
                struct expression *expr;
                double ans;
        } u;
        enum tag_e {
                EXPR,
                ANS
        } tag;
};

struct expression {
        struct expression *parent;
        struct Child **child;
        uint nchild;
        uint maxchild;
        enum Operators ops[NUM_OPS];
        uint numops;
        bool in_paren;
};


/*===========================================================================*/


#define streq(SA, SB)  (strcmp((SA), (SB)) == 0)
#define strneq(SA, SB) (strcmp((SA), (SB)) != 0)
#define modulo(A, B)   (((A) % (B) + (B)) % (B))
#define eprintf(...)   fprintf(stderr, __VA_ARGS__)

#define xeprintf(STATUS, ...)                 \
        do {                                  \
                fprintf(stderr, __VA_ARGS__); \
                exit(STATUS);                 \
        } while (0)

#define xasprintf(BUF, FMT, ...)                                                \
        do {                                                                    \
                if (asprintf((BUF), (FMT), __VA_ARGS__) < 0)                    \
                        xeprintf(2, "Error allocating memory for asprintf.\n"); \
        } while (0)

#define dump_list(LIST)                                       \
                for (uint32_t _i = 0; _i < (LIST)->num; ++_i) \
                        puts((LIST)->data[_i])                \


/*===========================================================================*/


/* main.c */
char *program_name;


/* utility.c */
#define   xatoi(STR)   __xatoi((STR), false)
#define   s_xatoi(STR) __xatoi((STR), true)
#define   nputs(STR)   fputs((STR), stdout)
char    * my_fgetline  (FILE *fp, int *len);
int       countlines   (const char *filename);
int64_t   __xatoi      (char *str, bool strict);
void    * xmalloc      (size_t size);
void    * xcalloc      (int num, size_t size);
void    * xrealloc     (void *ptr, size_t size);
#define   xfree(PTR)   do { if ((PTR) != NULL) free(PTR); } while (0)

struct str_list *getlines(const char *filename);
struct str_list *make_str_list(uint32_t numstrings);
void destroy_str_list(struct str_list *list);

/*void      print_array  (char **array, int len);*/
/*void      free_s_array (struct s_array *str_array);*/
/*void      xfree        (void *ptr);*/
/*void      shuffle      (char **array, size_t n);*/
/*void      pretty_print (uint32_t *intlist, uint32_t size);*/


/* options.c */
void handle_options(int argc, char **argv);


/* calcop.c */
struct Child      * replace_node   (struct expression *node, double num);
struct expression * add_expr       (struct expression *parent, bool in_paren);
struct expression * init_tree      ();
void                _add_operand   (struct expression *node, double num);
void                add_operator   (struct expression *node, int ch);
void                destroy_child  (struct Child *child);
void                destroy_tree   (struct expression *root);
void                insert_operand (struct expression *node, double num, uint index);


#endif /* fool.h */
