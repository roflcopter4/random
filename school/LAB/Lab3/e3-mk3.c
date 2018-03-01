#!/usr/bin/env ccsh
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define BAR "-----------"
#define LBAR "--------------------------------"
#define STRSIZE 64
/*#define endgame(x,y) printf("\n%s\n\n", LBAR); draw_board(y); printf("%s\n", x)*/

void draw_board(char *board);
int get_move(char *board, char player);
bool check_win(char *bd, char pl);

int main(void)
{
    int i, plIndex, ply;
    char msg[STRSIZE];
    char board[10];
    char players[] = "xo";
    bool retry = false;

    for (i = 1; i <= 9; i++)
        board[i] = ' ';

    plIndex = 0;
    ply = 1;
    while (true) {
        char cPlayer = players[plIndex];
        int move;
        /* No need to print the board if the player just made a mistake */
        if (!retry)
            draw_board(board);

        /* Get move and check if valid - function returns -1 if invalid */
        if ((move = get_move(board, cPlayer)) > 0) {
            board[move] = cPlayer;
            retry = false;
            /* Don't bother checking for a winner before it could even be possible */
            if (ply >= 5) {
                if (check_win(board, cPlayer)) {
                    sprintf(msg, "Congratulations! Player '%c' has won!", cPlayer);
                    break;
                } else if (ply == 9) { /* board only has 9 squares */
                    strncpy(msg, "The board is full - draw!", STRSIZE);
                    break;
                }
            }

            /* Select next player, increment ply counter, print bar */
            plIndex = (plIndex + 1) % 2;
            ply++;
            printf("\n%s\n\n", LBAR);
        } else
            retry = true;
    }

    /* Print final board and win/draw message */
    printf("\n%s\n\n", LBAR);
    draw_board(board);
    printf("%s\n", msg);

    return 0;
}


void draw_board(char *board)
{
    int i;
    for (i = 9; i > 0; i = i - 3) {
        /* Formatting the board 'by hand'. It makes sense when you stare at it long enough. */
        printf(" %c | %c | %c\t %d | %d | %d\n", board[i-2], board[i-1], board[i], i-2, i-1, i);
        if (i/3 > 1)
            printf("%s\t%s\n", BAR, BAR);
    }
    puts("");
}


int get_move(char *board, char player)
{
    int move, ch;
    char buf[1];
    bool blankline;

    printf("Player '%c', enter your move:  ", player);
    buf[0] = getchar();
    blankline = (buf[0] == '\n' || buf[0] == EOF);

    /* The fact that atoi returns 0 on an error isn't a problem here, given that 0 isn't
     * a valid entry anyway. Still, set move to -1 on errors just to be extra paranoid. */
    if ((move = atoi(buf)) <= 0 || move > 9) {
        /* Don't spit out the error message if the player just hit enter. */
        if (! blankline)
            puts("Invalid entry: input must be an integer between 1 and 9 (inclusive).");
        move = -1;
    } else if (board[move] != ' ') {
        puts("Invalid entry: square already filled.");
        move = -1;
    }

    /* Clear stdin, unless all the player entered was a blank line or a ctrl+d */
    if (! blankline)
        while ((ch = getchar()) != '\n' && ch != EOF) ;

    return move;
}


bool check_win(char *bd, char pl)
{
    /* Is there a non-hideous way of doing this?? This is just ugly. */
    int i;

    /* Check index 1,2,3 - 4,5,6 - 7,8,9 for horizontal wins */
    for (i = 1; i <= 9; i = i+3) {
        if (bd[i] == pl && bd[i+1] == pl && bd[i+2] == pl)
            return true;
    }

    /* Check index 1,4,7 - 2,5,8 - 3,6,9 for vertical wins */
    for (i = 1; i <= 3; i++) {
        if (bd[i] == pl && bd[i+3] == pl && bd[i+6] == pl)
            return true;
    }

    /* Ugliest of all. Check index 1,5,9 - 3,5,7 for diagonal wins */
    if (bd[1] == pl && bd[5] == pl && bd[9] == pl)
        return true;
    else if (bd[3] == pl && bd[5] == pl && bd[7] == pl)
        return true;


    return false;
}
