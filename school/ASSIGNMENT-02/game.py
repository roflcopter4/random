#!/usr/bin/env python3

import sys
import subprocess
import getopt
import random as rnd
from operator import concat


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
        self.row = row           # number of rows in grid
        self.col = col           # number of columns in grid
        self.initial = initial   # number of initial cells filled
        self.score = 0

        # creates the grid specified above
        self._grid = [[0]*col for _ in range(row)]
        # Creates the horizontal bar displated between rows in the grid.
        self.__hbar = '\t|%s\n' % (col*'-----|')

        self.length = row * col  # Avoid having to constantly calculate this.
        self.emptiesSet = list(range(self.length))  # list of empty cells
        self.new_cell = None  # The cell randomly filled in this turn.

        # assignation to two random cells
        for _ in range(self.initial):
            self.assign_rand_cell(init=True)

    # def __create_grid(self, row, col):
    #     """Returns a list of length row, of lists of length col, of zeroes."""
    #     return [[0]*col for _ in range(row)]

    def __ret_cell(self, cell):
        """Returns the index of the cell referred to by a number corresponding
        to the index that cell would have if the grid were a flat list.
        """
        a = cell // self.row % self.row
        b = cell % self.col
        return [a, b]

    def set_cell(self, cell, val):
        """Assigns 'val' to the cell in the flattened grid numbered 'cell'."""
        a, b = self.__ret_cell(cell)
        self._grid[a][b] = val

    def get_cell(self, cell):
        """Returns the value in cell number 'cell' of the flattened grid."""
        a, b = self.__ret_cell(cell)
        return self._grid[a][b]

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
        # I understand forcing the use of public methods to access private
        # attributes, but why can't the methods themselves directly access
        # them? It's more efficient to dereference the grid directly here.
        buf = self.__hbar
        for rInd in range(self.row):
            line = '\t|'
            for cInd in range(self.col):
                cell = self._grid[rInd][cInd]
                if not cell:
                    line += '%s|' % ' '.center(5)
                else:
                    this = ((rInd * self.row) + cInd)
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
        for i in range(self.length):
            if self.get_cell(i) == 0:
                self.emptiesSet.append(i)

    #
    def collapsible(self):
        """Returns whether the board is collapsible in any direction. """
        row = self.row  # The laziest and dumbest way to stay under 79 chars
        col = self.col
        for rInd in range(self.row):
            for cInd in range(self.col):
                cell = self._grid[rInd][cInd]
                if (cell == 0) or \
                   ((rInd+1 < row) and (cell == self._grid[rInd+1][cInd])) or \
                   ((cInd+1 < col) and (cell == self._grid[rInd][cInd+1])):
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
        ret = False
        for row in self._grid:
            row, tmp = self.collapseRow(row)
            ret = ret or tmp
        return ret

    def collapseRight(self):
        """Reverses each row and runs it through 'collapseRow', then reverses
        it again. Returns True if any rows collapsed to the right.
        """
        ret = False
        for row in self._grid:
            row.reverse()
            row, tmp = self.collapseRow(row)
            row.reverse()
            ret = ret or tmp
        return ret

    def collapseUp(self):
        """Gets a list for each column in the grid and runs it through
        collapseRow. The changed values in the list are then used to set the
        values in the main grid. Returns True if any columns collapsed upwards.
        """
        ret = False
        for cInd in range(self.col):
            lst = self.get_columns(cInd)
            lst, tmp = self.collapseRow(lst)
            ret = ret or tmp
            self.set_columns(lst, cInd)
        return ret

    def collapseDown(self):
        """Gets a list for each column in the grid, reverses it, runs it through
        collapseRow, and reverses it again. The changed values are used to set
        those in the main grid. Returns True if any columns collapsed downards.
        """
        ret = False
        for cInd in range(self.col):
            lst = self.get_columns(cInd)
            lst.reverse()
            lst, tmp = self.collapseRow(lst)
            lst.reverse()
            ret = ret or tmp
            self.set_columns(lst, cInd)
        return ret

    #
    # These two were separated as methods for additional clarity - they could
    # well have been left 'inline'.
    def get_columns(self, cInd):
        """Returns the value at index cInd for each row (ie sublist) in the
        grid, thus yielding a list of all values in a column.
        """
        return [self._grid[rInd][cInd] for rInd in range(self.row)]

    def set_columns(self, lst, cInd):
        """Sets each value in the column indicated by cInd in the grid to the
        corresponding value in lst.
        """
        x = 0
        for rInd in range(self.row):
            self._grid[rInd][cInd] = lst[x]
            x += 1

    #
    def print_grid(self, tile=None):
        """Prints the value of the grid or a square in it for debugging."""
        if tile is None:
            print(self._grid)
        else:
            tile = list(tile)
            print(self._grid[tile[0]][tile[1]])


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
            # key = input('\nEnter a move: ')
            key = getch()

            while key not in concat(list(self.moves.keys()), self.quit):
                self.print_prompt()
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
            print('\nScore: ' + str(self.grid.score))
            print('No more legal moves.')


###############################################################################
# Function Definitions


def main(testnum=None, rows=None, columns=None):
    if testnum is not None:
        do_test(testnum)
    else:
        kwargs = {}
        if rows is not None:
            kwargs['row'] = rows
        if columns is not None:
            kwargs['col'] = columns
        game = Game(**kwargs)
        game.play()


def clear_term():
    if sys.platform == 'win32':
        subprocess.call("cls")
    else:
        subprocess.call("clear")


def do_test(testnum):
    if testnum == 1:
        grid = Grid()
        grid.print_grid()

    elif testnum == 2:
        grid = Grid()
        grid.set_cell(6, 8)
        print(grid.get_cell(6))
        grid.print_grid()

    elif testnum == 3:
        grid = Grid(4, 4)
        grid.draw_grid()


def green(string):
    return "\033[0m\033[32m" + string + "\033[0m"


###############################################################################
# Init

OPTSTRING = "ht:s:r:c:"


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
            elif opt == 't':
                if optarg in (str(i) for i in range(1, 4)):
                    kwargs['testnum'] = int(optarg)
                else:
                    print("Unrecognized test number.")
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
-h	Show this help and exit
-t[N]	Run debug test number N (1-3 avail - all of dubious value)
-s[N]	Run the game with an NxN square grid
-r[N]	Run the game with N rows
-c[N]	Run the game with N columns

Unless otherwise specified the game is run on a 4x4 grid."""
          % (sys.argv[0], OPTSTRING))
    sys.exit()


if __name__ == "__main__":
    main(**check_opts())


# vim: ts=8
