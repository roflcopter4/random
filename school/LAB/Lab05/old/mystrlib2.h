#ifndef mystrlib2_h
#define mystrlib2_h
/* Pretty much all of these just crash the program at the first sign of trouble. */

/* ========================================================================== */
/* ========================================================================== */

/* Basic operations */
void myStrCpy(char **strto, char *strfrom);
void myStrCat(char **strto, char *str1, char *str2);
void myStrApp(char **strOrig, char *strToAppend);
void myStrFree(int count, ...);
void noMem();

/* Old string array functions. */
void getStrArray (char ***strArr, int numStrings);
void freeStrArray(char ***strArr, int numStrings);

void myStrInit(int count, ...);  /* Initializes strings to null */

/* Returns the number of lines in a file, and assigns to strArr an array of
 * strings containing the contents of the file, one line per string. */
int GetAllLines(char ***strArr, char *filename, int keepEmpty);

/* ========================================================================== */
/* ========================================================================== */


/* The new method I'd like to try - define everything with structs. */

typedef struct {
    char *str;
    size_t len;
} String;

void my_string_cpy(String *strTo, char *strFrom);
void my_string_cat(String *strto, char *str1, char *str2);
void my_string_app(String *str_orig, char *str_to_append);

void my_StringObj_cpy(String *strTo, String *strFrom);
void my_StringObj_cat(String *strto, String *str1, String *str2);
void my_StringObj_app(String *str_orig, String *str_to_append);

typedef struct{
    String *arr;
    int num;
} StringArray;

int get_all_lines(StringArray *str_array, char *filename);
void init_string_array(StringArray *str_array);
void free_string_array(StringArray *str_array);

#define STRINGARRAY(id) StringArray (id); (id).arr = NULL; (id).num = 0


/* ========================================================================== */
/* ========================================================================== */

/* my_getline() and my_fgetline -> safely get a full line from a file with no
 * risk of overflow, though at the cost ofsome overhead of course.
 * This function was heavily "inspired" by a generic anonymous one I read
 * somewhere. I hope it's trivial enough that this doesn't matter. */
#define my_getline(ln) my_fgetline(ln, stdin)
int my_fgetline(char **line, FILE *f);

/* ========================================================================== */
/* ========================================================================== */

/* Simple little utility macros. */
#define saferFree(x) do { if((x) != NULL) { free(x); (x) = NULL; } } while (0)
#define saferMalloc(x,p) do { if(((x) = malloc((p))) == NULL) noMem(); } while (0)
#define saferCalloc(x,p,s) do { if(((x) = calloc((p),(s))) == NULL) noMem(); } while (0)


#endif /* END mystrlib2.h */
