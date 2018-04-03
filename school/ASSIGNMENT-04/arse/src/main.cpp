/* 
   arse - blah

   Copyright (C) 2018 Brendan

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software Foundation,
   Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.  
*/

#include "arse.h"
#include <cstdio>
#include <cstdlib>

static inline uint8_t get_val(int argc, char **argv);
extern bool quiet;


int
main (int argc, char **argv)
{
        handle_options(argc, argv);
        uint8_t tokens = get_val(argc, argv);
        struct Node *root = init_tree(tokens);

        solve(root);
        if (!quiet)
                display(root);
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

        return static_cast<uint8_t>(val);
}
