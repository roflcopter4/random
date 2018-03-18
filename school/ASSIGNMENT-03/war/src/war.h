#ifndef src_war_h
#define src_war_h

/*===========================================================================*/
#if 0
#include "third_party/include/system.h"
#include "third_party/include/xmalloc.h"
extern char *xrealloc ();
extern char *xstrdup (); 
#endif
extern char *xmalloc ();
#ifndef VERSION
#  define VERSION "0.0.1"
#endif
/*===========================================================================*/

int decode_switches(int argc, char **argv, int *numcards);
char *program_name;

#include "mystrlib2.h"

void print_array(struct StringArray *data);
void validate_data(struct StringArray *data);


#endif /* war.h */
