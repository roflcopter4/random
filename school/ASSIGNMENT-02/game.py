#!/usr/bin/env python3

import sys
import subprocess
import getopt
import random as rnd
from operator import concat


###############################################################################
#
# NOTE: I take ZERO credit for this function. I copied it verbatim from a post
#       online. I figured that since it isn't even relevant to the real
#       excercize here and it makes it slightly more pleasant to test the
#       program that it would be ok to include it. If that is not the case, the
#       original two lines of code are just commented out and can be restored.
#
# SOURCE: https://stackoverflow.com/a/21659588/5511137

# Non blocking function allowing moves to be entered without a newline, just
# like getchar() in C.

def _find_getch():
    try:
        import termios
    except ImportError:
        # Non-POSIX. Return msvcrt's (Windows') getch.
        import msvcrt
        return msvcrt.getch

    # POSIX system. Create and return a getch that manipulates the tty.
    # import sys
    import tty

    def _getch():
        fd = sys.stdin.fileno()
        old_settings = termios.tcgetattr(fd)
        try:
            tty.setraw(fd)
            ch = sys.stdin.read(1)
        except EOFError:
            sys.exit()
        finally:
            termios.tcsetattr(fd, termios.TCSADRAIN, old_settings)
        return ch

    return _getch


getch = _find_getch()


###############################################################################
# Class Definitions


class Grid():
    def __init__(self, row=4, col=4, initial=2):
        self.collen = row       # No. of rows in grid (aka len of each column)
        self.rowlen = col       # No. of columns in grid (aka len of each row)
        self.initial = initial  # Number of initial cells filled.
        self.score = 0          # Player score

        self.new_cell = None             # The cell randomly filled this turn.
        self.length = row * col          # Avoid constantly recalculating this.
        self.Range = list(range(self.length))  # Ditto
        self.emptiesSet = self.Range.copy()    # List of empty cells.

        # Creates the specified grid with all cells set to 0.
        self._grid = [[0]*col for _ in range(row)]

        # Creates the horizontal bar displated between rows in the grid.
        self.__hbar = '\t|%s\n' % (col*'-----|')

        # assignation to two random cells
        for _ in range(self.initial):
            self.assign_rand_cell(init=True)

    #
    def __ret_cell(self, cell):
        """Returns the index of the cell referred to by a number corresponding
        to the index that cell would have if the grid were a flat list.
        """
        assert cell < self.length

        # Value increments after every rowlen * N, so work backwards.
        rInd = cell // self.rowlen
        # Values are consequtive and wrap at self.rowlen.
        cInd = cell % self.rowlen
        return rInd, cInd

    def set_cell(self, cell, val):
        """Assigns 'val' to the cell in the flattened grid numbered 'cell'."""
        rInd, cInd = self.__ret_cell(cell)
        self._grid[rInd][cInd] = val

    def get_cell(self, cell, dbg=False):
        """Returns the value in cell number 'cell' of the flattened grid."""
        rInd, cInd = self.__ret_cell(cell)
        return self._grid[rInd][cInd]

    #
    def assign_rand_cell(self, init=False):
        """Assigns a random empty cell of the grid a value of 2 or 4. When
        initializing only the number 2 is assigned; thereafter 2 and 4 are
        assigned 75% and 25% of the time, respectively.
        """
        if len(self.emptiesSet):
            cell = rnd.sample(self.emptiesSet, 1)[0]
            if init:
                self.set_cell(cell, 2)
            else:
                cdf = rnd.random()
                if cdf > 0.75:
                    self.set_cell(cell, 4)
                    self.new_val = 4
                else:
                    self.set_cell(cell, 2)
                    self.new_val = 2
                self.new_cell = cell
            self.emptiesSet.remove(cell)

    def draw_grid(self):
        """This function draws the grid representing the state of the game. I
        added a horizontal bar between each row and formatting to make the
        newest randomly assigned cell green.
        """
        buf = self.__hbar
        for rInd in range(self.collen):
            line = '\t|'
            for cInd in range(self.rowlen):
                this = ((rInd * self.rowlen) + cInd)
                cell = self.get_cell(this)
                if not cell:
                    line += '%s|' % ' '.center(5)
                else:
                    if this == self.new_cell:
                        tmp = green(str(cell).center(5))
                    else:
                        tmp = str(cell).center(5)
                    line += '%s|' % tmp
            buf += line + '\n' + self.__hbar
        print(buf)

    def updateEmptiesSet(self):
        """This function should update the list of empty cells of the grid."""
        self.emptiesSet = []
        for i in self.Range:
            if self.get_cell(i) == 0:
                self.emptiesSet.append(i)

    #
    # To test all possibilities it is only necessary to check cells forward and
    # down from the current cell, otherwise we'd pointlessly test cells twice.
    # * If any cell is 0 then the grid can collapse.
    # * Test whether each cell is equal to the one in front of it only if it is
    #    not at the end of a row, otherwise we'd test across to the next row.
    # * Test each cell vertically by testing the one at its index with index +
    #    rowlength only if its index does not place it in the bottom row,
    #    otherwise we'd test out of bounds.
    #
    def collapsible(self):
        """Returns true if the grid is collapsible in any direction."""
        rlen = self.rowlen
        selength = self.length  # Sleezy way to align the two conditions below
        for i in self.Range:
            cell = self.get_cell(i)
            if (cell == 0) \
               or ((i % rlen < rlen - 1) and (cell == self.get_cell(i + 1))) \
               or ((i + rlen < selength) and (cell == self.get_cell(i + rlen))):
                return True
        return False

    #
    # NOTE: Sorry for the walls of comments below. I thought the code was
    #       rather opaque but probably went too far here.
    #
    # 'base' is the index of either the start of the list (initially) or of the
    # last non-zero value to the left of the working index (i). It is
    # incremented only when another non-zero element is put in front of it,
    # meaning that if the current number collapses into and combines with the
    # number at base, we need to use an additional variable 'last_merge' to
    # avoid inadvertantly combining a second number into the same square in one
    # turn.
    #
    def collapseRow(self, lst):
        """Collapses the supplied list to the left. Returns whether the
        operation was successful. This method runs the show.
        """
        collapsible = False
        length = len(lst)
        base = zero_count = 0
        last_merge = None
        for i in range(length):
            if lst[i] == 0:
                zero_count += 1

            # Bad things happen if we test lst[base] with itself.
            elif base != i:
                # If the cells at i and base are equal and we haven't merged
                # anything onto base yet then do the merge, score up the
                # player, and record the merge. base is NOT incremented.
                if (lst[i] == lst[base]) and (base != last_merge):
                    lst[base] *= 2
                    self.score += lst[base]
                    lst[i] = 0
                    last_merge = base
                    collapsible = True

                # The value at lst[base] might initially be 0.
                elif (lst[base] == 0):
                    lst[base] = lst[i]
                    lst[i] = 0
                    collapsible = True

                # If i is base+1 and we've gotten here then they must not have
                # been able to merge, so there is nothing to do. i will only be
                # larger than base+1 if there is a 0 in between, meaning we can
                # move lst[i] to that 0 square. base will be incremented later.
                elif (i > base + 1):
                    lst[base+1] = lst[i]
                    lst[i] = 0
                    collapsible = True

                # Increment base if a non-zero value is in front of it.
                if lst[base+1] != 0:
                    base += 1

        # If all values were 0, consider the collapse successful (even though
        # we did nothing).
        if zero_count == length:
            collapsible = True

        return lst, collapsible

    #
    # These methods can be confusing. Note that collen = number of rows, and
    # rowlen = number of columns. Either 'rowlen' and 'collen' (as used) or
    # 'colnum' and 'rownum' would have been equivalent. I found naming by the
    # length of each element a little clearer.
    #
    # Finds the index of the start of each row in the conceptually flattened
    # array we're forced to work with, then gets a list of the indeces of the
    # cells in each row by taking a slice out of the range 0..length, of the
    # size of one row (rowlen). These are used to get a list of the values for
    # that row from get_cell().
    #
    def collapseLeft(self):
        """Sends each row to collapseRow. Returns True if any rows collapsed."""
        retval = False
        for StartInd in [i * self.rowlen for i in range(self.collen)]:
            cur_slice = self.Range[StartInd: StartInd + self.rowlen]
            lst = [self.get_cell(i) for i in cur_slice]
            lst, tmp = self.collapseRow(lst)
            for i in range(self.rowlen):
                self.set_cell(cur_slice[i], lst[i])
            retval = retval or tmp
        return retval

    def collapseRight(self):
        """Reverses each row and runs it through 'collapseRow', then reverses
        it again. Returns True if any rows collapsed to the right.
        """
        retval = False
        for StartInd in [i * self.rowlen for i in range(self.collen)]:
            cur_slice = self.Range[StartInd: StartInd + self.rowlen]
            lst = [self.get_cell(i) for i in cur_slice]
            lst.reverse()
            lst, tmp = self.collapseRow(lst)
            lst.reverse()
            for i in range(self.rowlen):
                self.set_cell(cur_slice[i], lst[i])
            retval = retval or tmp
        return retval

    #
    # These two work similarly to the above, but to get the values for the
    # columns we must skip rowlen * N to get to the next value down the column.
    # Further, the start of each column is simply a range from 0 to rowlen. The
    # list comprehension for getting the list of column values is split off to
    # a third method for a tiny increase in neatness.
    #
    def collapseUp(self):
        """Gets a list for each column in the grid and runs it through
        collapseRow. The changed values in the list are then used to set the
        values in the main grid. Returns True if any columns collapsed upwards.
        """
        retval = False
        for cStartInd in range(self.rowlen):
            lst = self.get_column(cStartInd)
            lst, tmp = self.collapseRow(lst)
            x = 0
            for i in range(cStartInd, self.length, self.rowlen):
                # self.set_cell(i, lst[x++])
                self.set_cell(i, lst[x])
                x += 1
            retval = retval or tmp
        return retval

    def collapseDown(self):
        """Gets a list for each column in the grid, reverses it, runs it through
        collapseRow, and reverses it again. The changed values are used to set
        those in the main grid. Returns True if any columns collapsed downards.
        """
        retval = False
        for cStartInd in range(self.rowlen):
            lst = self.get_column(cStartInd)
            lst.reverse()
            lst, tmp = self.collapseRow(lst)
            lst.reverse()
            x = 0
            for i in range(cStartInd, self.length, self.rowlen):
                self.set_cell(i, lst[x])
                x += 1
            retval = retval or tmp
        return retval

    def get_column(self, ind):
        """This method exists only to keep things a bit neater."""
        return [self.get_cell(i) for i in range(ind, self.length, self.rowlen)]


# ----------------------------------------------------------------------------


class Game():
    quit = ['p', 'Q']
    moves = {'w': 'Up',
             'a': 'Left',
             's': 'Down',
             'd': 'Right'}

    def __init__(self, row=4, col=4, initial=2):
        """Creates a game grid and begins the game"""
        self.grid = Grid(row, col, initial)

    def print_prompt(self):
        """Prints the instructions and the game grid with a move prompt."""
        clear_term()

        print('Press "w", "a", "s", or "d" to move Up, Left, Down or Right',
              'respectively.\nEnter "p" or "Q" to quit.\n')
        self.grid.draw_grid()
        print('\nScore: ' + str(self.grid.score))

    def play(self):
        collapsible = True

        # NOTE: The original function calls are both commented out here.
        while collapsible:
            self.print_prompt()
            # key = input('\nEnter a move: ')
            key = getch()

            while key not in concat(list(self.moves.keys()), self.quit):
                self.print_prompt()
                # key = input('\nEnter a move: ')
                key = getch()

            if key in self.quit:
                break
            else:
                move_p = getattr(self.grid, 'collapse' + self.moves[key])
                collapsed = move_p()
                if collapsed:
                    self.grid.updateEmptiesSet()
                    self.grid.assign_rand_cell()
                collapsible = self.grid.collapsible()

        if not collapsible:
            clear_term()
            print()
            self.grid.draw_grid()
            print('GAME OVER: No more legal moves.')
            print('\nScore: %d' % self.grid.score)
        else:
            print("\nGoodbye!\nFinal score: %d" % self.grid.score)


###############################################################################
# Function Definitions


def main(rows=None, columns=None):
    kwargs = {}
    if rows is not None:
        kwargs["row"] = rows
    if columns is not None:
        kwargs["col"] = columns
    game = Game(**kwargs)
    game.play()


def clear_term():
    """Clears the terminal in a platform independent manner."""
    if sys.platform == 'win32':
        subprocess.call("cls")
    else:
        subprocess.call("clear")


def green(string):
    """Turns a string green."""
    return "\033[0m\033[32m" + string + "\033[0m"


###############################################################################
# Init


OPTSTRING = "hs:r:c:"
LONGOPTS = ["help", "columns=", "rows=", "square="]


def check_opts():
    """Does what it says on the tin."""
    kwargs = {}
    try:
        opts, args = getopt.getopt(sys.argv[1:], OPTSTRING, LONGOPTS)
    except getopt.GetoptError as err:
        print(err)
        print("Aborting.")
        ShowUsage()
    else:
        for opt, optarg in opts:
            opt = opt.lstrip('-')
            if opt in ('h', "help"):
                ShowUsage()
            else:
                if opt in ('c', 's', "columns", "square"):
                    kwargs["columns"] = int(optarg)
                if opt in ('r', 's', "rows", "square"):
                    kwargs["rows"] = int(optarg)
    return kwargs


def ShowUsage():
    print("""\
Usage: %s -[%s] --[%s]
-h / help          Show this help and exit
-s / square=[N]    Run the game with an NxN square grid
-r / rows=[N]      Run the game with N rows
-c / columns=[N]   Run the game with N columns

Unless otherwise specified the game is run on a 4x4 grid."""
          % (sys.argv[0], OPTSTRING, ", ".join(LONGOPTS)))
    sys.exit()


if __name__ == "__main__":
    main(**check_opts())
