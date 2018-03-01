/* Pretty much all of these just crash the program at the first sign of trouble. */

void myStrCpy(char **strto, char *strfrom);
void myStrCat(char **strto, char *str1, char *str2);
void myStrApp(char **strOrig, char *strToAppend);
void myStrFree(int count, ...);

void getStrArray (char ***strArr, int numStrings);
void freeStrArray(char ***strArr, int numStrings);
void noMem();

/* Initializes strings to null */
void myStrInit(int count, ...);

/* Returns the number of lines in a file, and assigns to strArr an array of
 * strings containing the contents of the file, one line per string. */
int GetAllLines(char ***strArr, char *filename);

#define safeFree(x) if((x) != NULL) { free(x); (x) = NULL; }
#define saferCalloc(x,p,s) if(((x) = calloc((p),(s))) == NULL) noMem() 

/* END myStrLib.h */
