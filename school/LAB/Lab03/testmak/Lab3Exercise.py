#!/usr/bin/env python3

BAR = "-----------"
LONGBAR = BAR + "--------" + BAR

##############################################################################
# CLASSES


class TicTacToe:
    def __init__(self):
        """Ignore index 0"""
        self.board = [" "]*10
        self.board[0] = "#"

    def drawBoard(self):
        """Print the board with plays adjacent to a board with index."""
        bd = self.board
        for i in range(9, 1, -3):
            buf  = " %s | %s | %s " % (bd[i-2], bd[i-1], bd[i])
            buf += "\t"
            buf += " %d | %d | %d " % (i-2, i-1, i)
            print(buf)
            if i-2 > 1:
                print("%s\t%s" % (BAR, BAR))
        print()

    def boardFull(self):
        """Return whether the board is full."""
        try:
            self.board.index(" ")
            return False
        except ValueError:
            return True

    def cellIsEmpty(self, cell):
        """Return whether or not a square is empty."""
        return cell > 0 and cell < 10 and self.board[cell] == ' '

    def assignMove(self, cell, ch):
        """Simply set a cell to a given value."""
        self.board[cell] = ch

    def whoWon(self):
        """Check whether either player has won, and if so who."""
        if self.isWinner("x"):
            return "x"
        elif self.isWinner("o"):
            return "o"
        else:
            return None

    def isWinner(self, ch):
        """Check whether a given player has won."""
        bd = self.board

        # Check index 1,2,3 - 4,5,6 - 7,8,9 for horizontal wins
        for i in [1, 4, 7]:
            # if bd[i] == ch and bd[i+1] == ch and bd[i+2] == ch:
            if [bd[x] for x in [i, i+1, i+2]].count(ch) == 3:
                return True

        # Check index 1,4,7 - 2,5,8 - 3,6,9 for vertical wins
        for i in [1, 2, 3]:
            # if bd[i] == ch and bd[i+3] == ch and bd[i+6] == ch:
            if [bd[x] for x in [i, i+3, i+6]].count(ch) == 3:
                return True

        # Check index 1,5,9 - 3,5,7 for diagonal wins
        if [bd[1], bd[5], bd[9]].count(ch) == 3:
            return True
        elif [bd[3], bd[5], bd[7]].count(ch) == 3:
            return True

        return False


##############################################################################
# FUNCTIONS


def get_move(board, player):
    mov = input("Your move player '%s':  " % player)
    try:
        mov = int(mov)
        if board.cellIsEmpty(mov):
            board.assignMove(mov, player)
            return True
        else:
            print("Invalid entry: cell is full or input is out of bounds.")
            return False
    except ValueError or TypeError:
        if not mov == '':
            print("Invalid entry: input must be an integer.")
        return False


##############################################################################
# MAIN


def main():
    board = TicTacToe()
    ply = 1
    retry = False
    msg = ''

    players = ['x', 'o']
    cur_player_i = 0
    while True:
        cPlayer = players[cur_player_i]
        if not retry:
            board.drawBoard()
        if get_move(board, cPlayer):
            retry = False
            if ply >= 5:
                if board.isWinner(cPlayer):
                    msg = "Congratulations! Player '%s' wins!" % cPlayer
                    break
                # elif board.boardFull()
                elif ply == 9:
                    msg = "The board is full and we have no winner. Draw!"
                    break

            cur_player_i = (cur_player_i + 1) % 2
            ply += 1
            print("\n%s\n" % LONGBAR)
        else:
            retry = True

    print("\n%s\n" % LONGBAR)
    board.drawBoard()
    print(msg)


##############################################################################
main()
