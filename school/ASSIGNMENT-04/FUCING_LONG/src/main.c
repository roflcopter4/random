#include "tree.h"
#include <stdlib.h>

static inline uint8_t get_val(int argc, char **argv);


int
main(int argc, char **argv)
{
        program_name = argv[0];
        handle_options(argc, argv);
        uint8_t tokens = get_val(argc, argv);
        struct Node *root = init_tree(tokens);

        /* solve(root); */
        iter_solve(root);
        /* if (!quiet)
                display(root); */
        destroy_tree(root);
        return 0;
}


static inline uint8_t
get_val(int argc, char **argv)
{
        extern int optind;
        if (argc == optind)
                xeprintf(5, "Error: No value entered.\n");

        int64_t val = s_xatoi(argv[optind]);

        if (val <= 2)
                xeprintf(6, "Error: Value must be greater than 2.\n");

        return (uint8_t)val;
}
