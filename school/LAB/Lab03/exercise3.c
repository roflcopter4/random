#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define BAR "-----------"
#define LBAR "------------------------------"
/*#define endgame(x,y) printf("\n%s\n\n", LBAR); draw_board(y); printf("%s\n", x)*/

void draw_board(char *board);
int get_move(char *board, char player);
bool check_win(char *bd, char pl);

int main(void)
{
    int i, plIndex, ply;
    char msg[64];
    char board[10];
    char players[] = "xo";
    bool retry = false;

    for (i = 1; i <= 9; i++) {
        board[i] = ' ';
    }

    plIndex = 0;
    ply = 1;
    while (true) {
        char cPlayer = players[plIndex];
        int move;
        if (!retry)
            draw_board(board);

        if ((move = get_move(board, cPlayer)) > 0) {
            board[move] = cPlayer;
            retry = false;
            if (ply >= 5) {
                if (check_win(board, cPlayer)) {
                    sprintf(msg, "Congratulations! Player '%c' has won!", cPlayer);
                    /*endgame(msg, board);*/
                    break;
                } else if (ply == 9) {
                    strcpy(msg, "The board is full - draw!");
                    /*endgame(msg, board);*/
                    break;
                }
            }

            plIndex = (plIndex + 1) % 2;
            ply++;
            printf("\n%s\n\n", LBAR);
        } else
            retry = true;
    }

    printf("\n%s\n\n", LBAR);
    draw_board(board);
    printf("%s\n", msg);

    return 0;
}


void draw_board(char *board)
{
    int i;
    for (i = 9; i > 0; i = i - 3) {
        printf(" %c | %c | %c\t %d | %d | %d\n", board[i - 2], board[i - 1], board[i], i - 2, i - 1, i);
        if (i / 3 > 1)
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

    if ((move = atoi(buf)) <= 0 || move > 9) {
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

    for (i = 10; i >= 1; i = i-3) {
        if (bd[i] == pl && bd[i-1] == pl && bd[i-2] == pl)
            return true;
    }

    for (i = 1; i <= 3; i++) {
        if (bd[i] == pl && bd[i+3] == pl && bd[i+6] == pl)
            return true;
    }

    if (bd[1] == pl && bd[5] == pl && bd[9] == pl)
        return true;
    else if (bd[3] == pl && bd[5] == pl && bd[7] == pl)
        return true;


    return false;
}
