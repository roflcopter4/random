#ifndef _GNU_SOURCE
#  define _GNU_SOURCE
#endif
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "war.h"
int compare(const void *a, const void *b);


#define VALUES "AKQJ023456789"
#define SUITS "CDHS"

void
validate_data(struct s_array *data)
{
        /* The deck must have exactly (D_SIZE) cards. */
        if (data->num != D_SIZE) {
                eprintf("ERROR: Deck is the wrong size.\n");
                exit(1);
        }

        /* Every card must be exactly (C_SIZE) characters */
        for (int i = 0; i < D_SIZE; ++i)
                if (data->len[i] != (C_SIZE)) {
                        eprintf("ERROR: Card '%s' is the wrong size.\nAll cards"
                                " must be exactly %d characters long.\n",
                                data->arr[i], C_SIZE);
                        exit(2);
                }

        /* The deck must not contain duplicate cards. The easiest way to check
         * that is to sort the deck and check each card with its neighbour. */
        qsort(data->arr, D_SIZE, sizeof(char *), compare);
        for (int i = 0; i < (D_SIZE - 1); ++i)
                if (streq(data->arr[i], data->arr[i+1])) {
                        eprintf("ERROR: Deck contains duplicate cards.\n");
                        exit(3);
                }

        /* Now check that we have every card. I'm sure the O complexity of this
         * implementation is just fantastic. 3 stacked for loops. Classy. */
        char card_vals[2][32] = {VALUES, SUITS};
        int len[2]            = {strlen(VALUES), strlen(SUITS)};

        for (int i = 0; i < (D_SIZE - 1); ++i) {
                int found = 0;
                for (int pos = 0; pos < 2; ++pos)
                        for (int ch = 0; ch < len[pos]; ++ch)
                                if (data->arr[i][pos] == card_vals[pos][ch]) {
                                        ++found;
                                        break;
                                }
                if (found != 2) {
                        eprintf("ERROR: Card '%s' is invalid.\n", data->arr[i]);
                        exit(4);
                }
        }
}


int
compare(const void *a, const void *b)
{
        return strcmp(*(const char **)a,
                      *(const char **)b);
}
