#include "tree.h"
#include <stdlib.h>

static inline int get_val(int argc, char **argv);


int
main (int argc, char **argv)
{
        program_name = argv[0];
        decode_switches(argc, argv);
        int tokens = get_val(argc, argv);
        struct Node *root = init_tree(tokens);

        solve(root);
        display(root);
        destroy_tree(root);

        return 0;
}


static inline int
get_val(int argc, char **argv)
{
        extern int optind;
        if (argc == optind)
                xeprintf(5, "Error: No value entered.\n");
        return xatoi(argv[optind]);
}
