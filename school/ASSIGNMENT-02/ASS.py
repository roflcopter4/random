#!/usr/bin/env python3

import sys
import subprocess
import getopt
import random as rnd
from operator import concat

from time import sleep
import traceback


msg = ''


###############################################################################
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
    import sys
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
        self.row = row           # Number of rows in grid.
        self.col = col           # Number of columns in grid.
        self.initial = initial   # Number of initial cells filled.
        self.score = 0           # Player score

        self.length = row * col          # Avoid constantly recalculating this.
        # print("Row: %d\nCol: %d\nLen: %d\n" % (self.row, self.col, self.length))
        # sleep(3)
        self.Range = list(range(self.length))  # Ditto
        self.emptiesSet = self.Range.copy()    # List of empty cells.
        self.new_cell = None             # The cell randomly filled this turn.

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
        to the index that cell would have if the grid were a flat list. The
        program crashes if is greater than the length of the grid. This
        shouldn't ever happen, but Safety First.
        """
        # try:
        #     assert cell < self.length
        # except AssertionError:
        #     print("ERROR: Cell value '%d' is larger than total size '%d'." % (cell, self.length-1))
        #     sys.exit()
        # Next value is every self.row * N. Work backwards with int division.
        # a = cell // self.row % self.row
        # a = cell // self.row
        a = cell // self.col % self.row
        # Values are consequtive and wrap at self.col.
        b = cell % self.col
        return [a, b]

    #
    def set_cell(self, cell, val):
        """Assigns 'val' to the cell in the flattened grid numbered 'cell'."""
        a = b = 0
        try:
            a, b = self.__ret_cell(cell)
            self._grid[a][b] = val
        except IndexError as e:
            self.perror("Error: '%s'." % e, cell, a, b, 5)
            self.perror("Error.", cell, a, b, 5)
            sys.exit()

    def get_cell(self, cell, dbg=False):
        """Returns the value in cell number 'cell' of the flattened grid."""
        a = b = 0
        try:
            a, b = self.__ret_cell(cell)
            if dbg:
                return self._grid[a][b], a, b
            else:
                return self._grid[a][b]
        except IndexError as e:
            self.perror("Error: '%s'." % e, cell, a, b, 5)
            sys.exit()

    def perror(self, e, cell, a, b, s=0.5):
        traceback.print_stack()
        print("\n%s\ncell: %d - len %d\na: %d - amax: %d\nb: %d - bmax %d"
              % (e, cell, self.length-1, a, self.row-1, b, self.col-1))
        print(self._grid)
        # sleep(s)
        sys.exit()

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

    #
    def draw_grid(self):
        """This function draws the grid representing the state of the game."""
        buf = self.__hbar
        for rInd in range(self.row):
            line = '\t|'
            for cInd in range(self.col):
                this = ((rInd * self.col) + cInd)
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
    def collapsible(self):
        """Good lord it's become lisp."""
        row = self.row
        col = self.col
        slength = self.length  # Sleezy way to align the two conditions below
        for i in self.Range:
            cell = self.get_cell(i)
            if (cell == 0) \
               or ((i % row < row - 1) and (cell == self.get_cell(i + 1))) \
               or ((i + col < slength) and (cell == self.get_cell(i + col))):
                return True
        return False

    #
    def collapseRow(self, lst):
        """Collapses the supplied list to the left. Returns whether the
        operation was successful. This method runs the show.
        """
        collapsible = False
        length = len(lst)
        base = ecount = 0
        last_merge = None
        for i in range(length):
            if lst[i] == 0:
                ecount += 1
            elif base != i:
                if lst[i] == lst[base] and base != last_merge:
                    lst[base] *= 2
                    self.score += lst[base]
                    lst[i] = 0
                    last_merge = base
                    collapsible = True
                elif lst[base] == 0:
                    lst[base] = lst[i]
                    lst[i] = 0
                    collapsible = True
                elif i > base+1:
                    lst[base+1] = lst[i]
                    lst[i] = 0
                    collapsible = True

                if lst[base+1] != 0:
                    base += 1
        if ecount == length:
            collapsible = True

        return lst, collapsible

    #
    def collapseLeft(self):
        """Wrapper around 'collapseRow'. Returns True if any rows collapsed."""
        retval = False
        for rStartInd in [i * self.col for i in range(self.row)]:
            cSlice = self.Range[rStartInd: rStartInd + self.col]
            lst = [self.get_cell(i) for i in cSlice]
            lst, tmp = self.collapseRow(lst)
            for i in range(self.col):
                self.set_cell(cSlice[i], lst[i])
            retval = retval or tmp
        return retval

    def collapseRight(self):
        """Reverses each row and runs it through 'collapseRow', then reverses
        it again. Returns True if any rows collapsed to the right.
        """
        retval = False
        for rStartInd in [i * self.col for i in range(self.row)]:
            cSlice = self.Range[rStartInd: rStartInd + self.col]
            lst = [self.get_cell(i) for i in cSlice]
            lst.reverse()
            lst, tmp = self.collapseRow(lst)
            lst.reverse()
            for i in range(self.col):
                self.set_cell(cSlice[i], lst[i])
            retval = retval or tmp
        return retval

    #
    def collapseUp(self):
        """Gets a list for each column in the grid and runs it through
        collapseRow. The changed values in the list are then used to set the
        values in the main grid. Returns True if any columns collapsed upwards.
        """
        retval = False
        for cStartInd in range(self.col):
            lst = [self.get_cell(i) for i in range(cStartInd, self.length, self.col)]
            lst, tmp = self.collapseRow(lst)
            x = 0
            for i in range(cStartInd, self.length, self.col):
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
        for cStartInd in range(self.col):
            lst = [self.get_cell(i) for i in range(cStartInd, self.length, self.col)]
            lst.reverse()
            lst, tmp = self.collapseRow(lst)
            lst.reverse()
            x = 0
            for i in range(cStartInd, self.length, self.col):
                self.set_cell(i, lst[x])
                x += 1
            retval = retval or tmp
        return retval


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
        stop = False
        collapsible = True

        while not stop and collapsible:
            self.print_prompt()
            print(self.grid._grid)
            # key = input('\nEnter a move: ')
            key = getch()

            while key not in concat(list(self.moves.keys()), self.quit):
                self.print_prompt()
                print(self.grid._grid)
                # key = input('\nEnter a move: ')
                key = getch()

            if key in self.quit:
                stop = True
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


def main(testnum=None, rows=None, columns=None):
    kwargs = {}
    if rows is not None:
        kwargs['row'] = rows
    if columns is not None:
        kwargs['col'] = columns
    game = Game(**kwargs)
    # print(game.grid._grid)
    # for i in game.grid.Range:
    #     c, a, b = game.grid.get_cell(i, True)
    #     print("%d -> %d  -- a: %d, b: %d" % (i, c, a, b))
    game.play()


def clear_term():
    if sys.platform == 'win32':
        subprocess.call("cls")
    else:
        subprocess.call("clear")


def green(string):
    return "\033[0m\033[32m" + string + "\033[0m"


###############################################################################
# Init

OPTSTRING = "hs:r:c:"


def check_opts():
    kwargs = {}
    try:
        opts, args = getopt.getopt(sys.argv[1:], OPTSTRING)
    except getopt.GetoptError as err:
        print(err)
        print("Aborting.")
        ShowUsage()
    else:
        for opt, optarg in opts:
            opt = opt.lstrip('-')
            if opt == 'h':
                ShowUsage()
            elif opt in ('s', 'r', 'c'):
                if opt in ('r', 's'):
                    kwargs['rows'] = int(optarg)
                if opt in ('c', 's'):
                    kwargs['columns'] = int(optarg)
    return kwargs


def ShowUsage():
    print("""\
Usage: %s -[%s]
-h      Show this help and exit
-s[N]   Run the game with an NxN square grid
-r[N]   Run the game with N rows
-c[N]   Run the game with N columns

Unless otherwise specified the game is run on a 4x4 grid."""
          % (sys.argv[0], OPTSTRING))
    sys.exit()


if __name__ == "__main__":
    main(**check_opts())
