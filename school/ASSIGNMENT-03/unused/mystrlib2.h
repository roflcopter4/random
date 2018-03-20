#ifndef mystrlib2_h
#define mystrlib2_h
/* Pretty much all of these just crash the program at the first sign of trouble. */


/* ========================================================================== */
/* ========================================================================== */

#include <stdio.h>

/* The new method I'd like to try - define everything with structs. */

/* String object - contains a c ctring and its length. */
struct String {
        char *str;
        size_t len;
};

/* String array - contains an array of String objects and the number of them. */
struct StringArray {
        struct String *arr;
        int num;
};


/* Copy, concatenate, append a string to a String object. */
void my_strcpy(struct String *strTo, char *strFrom);
void my_strcat(struct String *strto, char *str1, char *str2);
void my_strapp(struct String *str_orig, char *str_to_append);

/* Copy, concatenate, append two struct String objects. */
void my_strtcpy(struct String *strTo, struct String *strFrom);
void my_strtcat(struct String *strto, struct String *str1, struct String *str2);
void my_strtapp(struct String *str_orig, struct String *str_to_append);

int get_all_lines(struct StringArray *str_array, char *filename);
void get_stdin(struct StringArray *str_array);
void SA_init(struct StringArray *str_array);
void SA_free(struct StringArray *str_array);

struct StringArray SA_append(struct StringArray *str_array, char *str);
struct StringArray SA_append_string(struct StringArray *str_array, struct String str);


/* ========================================================================== */
/* ========================================================================== */


/* my_getline() and my_fgetline -> safely get a full line from a file with no
 * risk of overflow, though at the cost ofsome overhead of course.
 * This function was heavily "inspired" by a generic anonymous one I read
 * somewhere. I hope it's trivial enough that this doesn't matter. */
int my_fgetline(char **line, FILE *f);
#define my_getline(ln) my_fgetline(ln, stdin)


/* ========================================================================== */
/* ========================================================================== */


void noMem();

/* Simple little utility macros. */
#define my_free(x)                                    \
        do {                                          \
                if ((x) != NULL) {                    \
                        free(x);                      \
                        (x) = NULL;                   \
                }                                     \
        } while (0)
#define my_xmalloc(x, p)                              \
        do {                                          \
                if (((x) = malloc((p))) == NULL)      \
                        noMem();                      \
        } while (0)
#define my_xcalloc(x, p, s)                           \
        do {                                          \
                if (((x) = calloc((p), (s))) == NULL) \
                        noMem();                      \
        } while (0)


#endif /* END mystrlib2.h */
