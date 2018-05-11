#include <alloca.h>
#include <bsd/bsd.h>
#include <bstrlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define dataof(_BSTR_) ((char *)((_BSTR_)->data))
#define fuck(f) ((char *)(f))


int
main(int argc, char **argv)
{
        if (argc == 1)
                exit(1);

        struct bstrList *lst = bstrListCreate();

        /* printf("%d - %d - %0zu\n", lst->qty, lst->mlen, lst->entry); */

        for (char **ptr = argv; *ptr; ++ptr) {
                if (lst->qty == lst->mlen)
                        bstrListAlloc(lst, lst->mlen + 20);
                lst->entry[lst->qty++] = bfromcstr(*ptr);
        }

        for (int i = 0; i < lst->qty; ++i)
                puts(dataof(lst->entry[i]));

        bstrListDestroy(lst);

        bstring whore = bfromcstr("You will die.");
        bcatcstr(whore, " In fucking flames.");
        bformata(whore, " Seriously die die die die %u times die\n", 1000);
        bJustifyCenter(whore, 100, ' ');

        write(STDOUT_FILENO, bdata(whore), blength(whore));
        /* bdestroy(whore); */

        return 0;
}
