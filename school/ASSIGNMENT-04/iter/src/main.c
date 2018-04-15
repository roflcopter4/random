#include "tree.h"
#include <limits.h>
#include <stdlib.h>

static inline uint8_t get_val(int argc, char **argv);


int
main(int argc, char **argv)
{
        program_name = argv[0];
        handle_options(argc, argv);

        uint8_t tokens    = get_val(argc, argv);
        struct Node *root = init_tree(tokens);

        solve(root);
        destroy_tree(root);

        return 0;
}


static inline uint8_t
get_val(int argc, char **argv)
{
        const char *errptr;
        extern int optind;

        if (argc == optind)
                xeprintf(1, "Error: No value entered.\n");

        int64_t val = strtonum(argv[optind], 2, UCHAR_MAX, &errptr);
        if (errptr)
                xeprintf(2, "Error: argument is %s.\n", errptr);

        return (uint8_t)val;
}
