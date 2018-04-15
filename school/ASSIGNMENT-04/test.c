#include <stdio.h>
#include <bsd/bsd.h>

int main(void)
{
        char *thing = "HELLO FAGGOT WHORE NIGGER TURD";
        char bumfuck[BUFSIZ];

        strlcpy(bumfuck, thing, BUFSIZ);

        printf("%s --> %s\n", thing, bumfuck);

        return 0;
}
