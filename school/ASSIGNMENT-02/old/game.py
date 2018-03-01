#!/usr/bin/env python3

import sys
import subprocess
import getopt
import random as rnd

# v = True
v = False

###############################################################################
# Class Definitions


class Grid():
    def __init__(self, row=4, col=4, initial=2):
        self.row = row           # number of rows in grid
        self.col = col           # number of columns in grid
        self.initial = initial   # number of initial cells filled
        self.score = 0

        # creates the grid specified above
        self._grid = self.__create_grid(row, col)
        self.__hbar = '\t|%s\n' % (col*'-----|')
        self.__len = row * col
        self.emptiesSet = list(range(self.__len))  # list of empty cells
        print(self.emptiesSet)

        # assignation to two random cells
        for _ in range(self.initial):
            self.assign_rand_cell(init=True)

    def __create_grid(self, row, col):
        """Returns a list of length row, of lists of length col, of zeroes."""
        return [[0]*col for _ in range(row)]

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
        """This function assigns a random empty cell of the grid a value of 2 or 4.
        * In __init__() it only assigns cells the value of 2.
        * The distribution is set so that 75% of the time the random cell is
          assigned a value of 2 and 25% of the time a random cell is assigned
          a value of 4
        """
        if len(self.emptiesSet):
            cell = rnd.sample(self.emptiesSet, 1)[0]
            if init:
                self.set_cell(cell, 2)
            else:
                cdf = rnd.random()
                if cdf > 0.75:
                    self.set_cell(cell, 4)
                else:
                    self.set_cell(cell, 2)
            self.emptiesSet.remove(cell)

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
                    line += '%s|' % str(cell).center(5)
            buf += line + '\n' + self.__hbar
        print(buf)

    def update_empties_set(self):
        """This function should update the list of empty cells of the grid."""
        self.emptiesSet = []
        for i in range(self.__len):
            if self.get_cell(i) == 0:
                self.emptiesSet.append(i)

    def collapsible(self):
        """Returns whether the board is collapsible in any direction. """
        for rInd in range(self.row):
            for cInd in range(self.col):
                cell = self._grid[rInd][cInd]
                if cell == 0:
                    return True
                elif (rInd+1 < self.row) and (cell == self._grid[rInd+1][cInd]):
                    return True
                elif (cInd+1 < self.col) and (cell == self._grid[rInd][cInd+1]):
                    return True
        return False

#
    # This function takes a list lst and collapses it to the LEFT.
    # This function should return two values:
    #     1. the collapsed list and
    #     2. True if the list is collapsed and False otherwise.
    def collapseRow(self, lst):
        collapsible = False
        length = len(lst)
        base = ecount = 0
        lm = None
        for i in range(length):
            if v:
                print(lst)
                print("     i = %d,      base = %d" % (i, base))
                print("lst[i] = %d, lst[base] = %d" % (lst[i], lst[base]))
                print()
            if lst[i] == 0:
                ecount += 1
            elif base != i:
                if lst[i] == lst[base] and base != lm:
                    lst[base] *= 2
                    lst[i] = 0
                    lm = base
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
        if v:
            print(lst)
            print()
        if ecount == len(lst):
            collapsible = True

        return lst, collapsible

#
    def collapseLeft(self):
        ret = False
        for row in self._grid:
            row, tmp = self.collapseRow(row)
            ret = ret or tmp
        return ret

    def collapseRight(self):
        ret = False
        for row in self._grid:
            row.reverse()
            row, tmp = self.collapseRow(row)
            ret = ret or tmp
            row.reverse()
        return ret

    def collapseUp(self):
        ret = False
        for i in range(0, self.row):
            lst = [self.get_cell(i+n) for n in range(0, self.__len, self.col)]
            lst, tmp = self.collapseRow(lst)
            ret = ret or tmp
            m = 0
            for n in range(0, self.col*self.row, self.col):
                self.set_cell(i+n, lst[m])
                m += 1
        return ret

    def collapseDown(self):
        ret = False
        for i in range(0, self.row):
            lst = [self.get_cell(i+n) for n in range(0, self.__len, self.col)]
            lst.reverse()
            lst, tmp = self.collapseRow(lst)
            lst.reverse()
            ret = ret or tmp
            m = 0
            for n in range(0, self.col*self.row, self.col):
                self.set_cell(i+n, lst[m])
                m += 1
        return ret

#
    def print_grid(self, tile=None):
        """Prints the value of the whole grid or of a square in it for debugging."""
        if tile is None:
            print(self._grid)
        else:
            tile = list(tile)
            print(self._grid[tile[0]][tile[1]])


# ----------------------------------------------------------------------------


class Game():
    def __init__(self, row=4, col=4, initial=2):
        """Creates a game grid and begins the game"""
        self.grid = Grid(row, col, initial)
        self.play()

    def print_prompt(self):
        """Prints the instructions and the game grid with a move prompt."""
        clear_term()

        print('Press "w", "a", "s", or "d" to move Up, Left, Down or Right respectively.')
        print('Enter "p" to quit.\n')
        self.grid.draw_grid()
        print('\nScore: ' + str(self.grid.score))

    def play(self):
        moves = {'w': 'Up',
                 'a': 'Left',
                 's': 'Down',
                 'd': 'Right'}

        stop = False
        collapsible = True

        while not stop and collapsible:
            self.print_prompt()
            key = input('\nEnter a move: ')

            while key not in list(moves.keys()) + ['p']:
                self.print_prompt()
                key = input('\nEnter a move: ')

            if key == 'p':
                stop = True
            else:
                move = getattr(self.grid, 'collapse' + moves[key])
                collapsed = move()

                if collapsed:
                    self.grid.update_empties_set()
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


def main(testnum):
    if testnum is not None:
        do_test(testnum)
    else:
        game = Game()
        if game is not None:
            pass


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


###############################################################################
# Init


def check_opts():
    OPTSTRING = "ht:"
    testnum = None
    try:
        opts, args = getopt.getopt(sys.argv[1:], OPTSTRING)
    except getopt.GetoptError as err:
        print(err)
        print("Aborting and reverting to default behavior.")
    else:
        for opt, optarg in opts:
            opt = opt.lstrip('-')
            if opt == 'h':
                print(OPTSTRING)
                sys.exit()
            elif opt == 't':
                if optarg in (str(i) for i in range(1, 6)):
                    testnum = int(optarg)
                else:
                    print("Unrecognized test number.")
                    sys.exit()
    finally:
        return testnum


if __name__ == "__main__":
    main(check_opts())
