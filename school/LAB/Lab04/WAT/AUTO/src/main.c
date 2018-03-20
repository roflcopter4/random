#include <stdio.h>
#include <stdlib.h>
#include "mystrlib2.h"

#define FILENAME "solvable_maze.txt"

int main(void)
{
    int it;
    StringArray str_array;
    String string1, string2;
    str_array.arr = NULL;
    str_array.num = 0;
    string1.str = string2.str = NULL;

    if (get_all_lines(&str_array, FILENAME))
        return 3;

    for (it = 0; it < str_array.num; ++it)
    {
        printf("Line no %2d is:\t\t%s\t\tand its size is %lu.\n", it, str_array.arr[it].str, str_array.arr[it].len);
    }

    my_string_cpy(&string1, "CUNTFACE");
    puts(string1.str);

    my_stringObj_cpy(&string2, &string1);
    puts(string2.str);

    saferFree(string1.str);
    saferFree(string2.str);
    free_string_array(&str_array);
    return 0;
}
