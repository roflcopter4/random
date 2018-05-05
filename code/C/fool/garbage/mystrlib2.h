#ifndef mystrlib2_h
#define mystrlib2_h
/* Pretty much all of these just crash the program at the first sign of trouble. */


/* ========================================================================== */
/* ========================================================================== */

#include <stdio.h>

/* The new method I'd like to try - define everything with structs. */

typedef struct {
        char *str;
        size_t len;
} String;

typedef struct {
        String *arr;
        int num;
} StringArray;


void my_string_cpy(String *strTo, char *strFrom);
void my_string_cat(String *strto, char *str1, char *str2);
void my_string_app(String *str_orig, char *str_to_append);

void my_StringObj_cpy(String *strTo, String *strFrom);
void my_StringObj_cat(String *strto, String *str1, String *str2);
void my_StringObj_app(String *str_orig, String *str_to_append);

int get_all_lines(StringArray *str_array, char *filename);
void get_stdin(StringArray *str_array);
void SA_init(StringArray *str_array);
void SA_free(StringArray *str_array);

StringArray SA_append(StringArray *str_array, char *str);
StringArray SA_append_string(StringArray *str_array, String str);

#define STRINGARRAY(id)  \
        StringArray(id); \
        (id).arr = NULL; \
        (id).num = 0


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
#define saferFree(x)                                  \
        do {                                          \
                if ((x) != NULL) {                    \
                        free(x);                      \
                        (x) = NULL;                   \
                }                                     \
        } while (0)
#define saferMalloc(x, p)                             \
        do {                                          \
                if (((x) = malloc((p))) == NULL)      \
                        noMem();                      \
        } while (0)
#define saferCalloc(x, p, s)                          \
        do {                                          \
                if (((x) = calloc((p), (s))) == NULL) \
                        noMem();                      \
        } while (0)


#endif /* END mystrlib2.h */
