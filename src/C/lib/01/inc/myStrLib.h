void myStrCpy(char** /* strto */   , char* /* strfrom */                      );
void myStrCat(char** /* strto */   , char* /* str1 */ ,      char* /* str2 */ );
void myStrApp(char** /* strOrig */ , char* /* strToAppend */                  );
void myStrFree(int   /* count */   , ...                                      );

void getStrArray (char*** /* strArr */ , int /* numStrings */);
void freeStrArray(char*** /* strArr */ , int /* numStrings */);


/* NEWER */

/* Initializes strings to null */
void initStr(int /* count */, ... /* strings */);

/* Returns the number of lines in a file, and assigns to strArr an array of
 * strings containing the contents of the file, one line per string. */
int GetAllLines(char *** /* strArr */, char * /* filename */);
