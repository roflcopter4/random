#include <stdio.h>
#include <stdlib.h>
#include "mystrlib2.h"

#define FILENAME "solvable_maze.txt"
#define UNSH unsigned short

void print_file(StringArray *str_array);
void parse_start(UNSH int *vec, StringArray *str_array, int i);

UNSH int *ps_st(StringArray *, int);

typedef struct {
    UNSH int legmoves[4][2];
    UNSH int location[2];
} MazeSquare;

typedef struct {
    /*UNSH int *size;*/
    UNSH int size[2];
    MazeSquare *squares;
    MazeSquare start;
    MazeSquare end;
} Maze;
void init_maze(Maze *maze, StringArray *str_array);

int main(void)
{
    int it;
    StringArray str_array;
    Maze maze;

    str_array.arr = NULL;
    str_array.num = 0;
    if (get_all_lines(&str_array, FILENAME))
        return 3;

    init_maze(&maze, &str_array);
    /*for (it = 0; it < str_array.num; ++it) {*/

    /*}*/

    free_string_array(&str_array);
    return 0;
}


void print_file(StringArray *str_array)
{
    int it;
    for (it = 0; it < str_array->num; ++it) {
        printf("Line no %2d is:\t\t%s\t\tand its size is %lu.\n", it,
                str_array->arr[it].str, str_array->arr[it].len);
    }
}


void init_maze(Maze *maze, StringArray *str_array)
{
    int i;
    UNSH int start_loc[2];
    UNSH int end_loc[2];
    parse_start(maze->size, str_array, 0);
    parse_start(start_loc, str_array, 1);
    parse_start(end_loc, str_array, 2);

    maze->squares = calloc(str_array->num, sizeof(MazeSquare));

    for (i=3; i < str_array->num; ++i)
    {
        int ax, x, y;
        ax = 0;
        char *str = str_array->arr[i].str;
        MazeSquare *square = &maze->squares[i];

        for (x = 0; x < )

        /*
         *while ((ch = str_array->arr[i].str[n]) != '\n' && ch != EOF && ch != '\r')
         *{
         *    if (ch != ' ') {
         *        maze->squares[n][ax] = atoi(&str_array->arr[i].str[n]);
         *    }
         *}
         */
    }
}

void parse_start(UNSH int *vec, StringArray *str_array, int i)
{
    vec[0] = atoi(&(str_array->arr[i].str[0]));
    vec[1] = atoi(&(str_array->arr[i].str[2]));
}

