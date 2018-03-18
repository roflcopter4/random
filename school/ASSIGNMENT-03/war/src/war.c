#include <assert.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "war.h"

static void parse_file(struct StringArray *data, int argc,
                       char **argv, int optind);

int main(int argc, char **argv)
{
        int optind, numcards;
        struct StringArray data = {NULL, 0};
        numcards = 0;
        program_name = argv[0];

        optind = decode_switches(argc, argv, &numcards);
        parse_file(&data, argc, argv, optind);

        validate_data(&data);

        /*for (int i = 0; i < data.num; ++i) {
         *        printf("%s\n", data.arr[i].str);
         *}*/

        SA_free(&data);
}


static void
parse_file(struct StringArray *data,
           int argc, char **argv, int optind)
{
        /*char *buf = NULL;*/
        int i;
        char buf[] = "/home/bml/random/school/ASSIGNMENT-03/shuffledDeck.txt";

        /*if (argc > optind)*/
                /*buf = argv[optind];*/
        /*else*/

                /*my_getline(&buf);*/

        if (get_all_lines(data, buf) != 0)
                exit(EXIT_FAILURE);

        for (i = 0; i < data->num; ++i) {
                int m = 0;
                char *c;
                while (*(c = &data->arr[i].str[m++]))
                        *c = toupper(*c);
        }
}

