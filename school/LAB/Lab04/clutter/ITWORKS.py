#!/usr/bin/env python3

import sys

FILES = ["solvable_maze.txt", "unsolvable_maze.txt", "unsolvable_maze2.txt"]
NORTH = 0
EAST = 1
SOUTH = 2
WEST = 3
X_STR = "│  "
Y_STR = "───"
X_PAD = "   "
Y_PAD = "   "

##############################################################################
# CLASSES


class Stack:
    def __init__(self):
        self.__items = []

    def push(self, item):
        self.__items.append(item)

    def pop(self):
        return self.__items.pop()

    def peek(self):
        return self.__items[-1]

    def is_empty(self):
        return len(self.__items) == 0

    def size(self):
        return len(self.__items)

    def clear(self):
        self.__items = []

    def debug_print(self):
        print(self.__items)


class Maze:
    def __init__(self, file_name):
        self.__start = None
        self.__finish = None
        self.__squares = []
        try:
            fp = open(filename, "r")
            maze_size = self.parse_line(fp.readline())

            # Create the double array of zeroes. [x*[y*<0>]]
            for x in range(0, maze_size[0]):
                self.__squares.append(maze_size[1] * [0])

            # Make a MazeSquare object for every 0 in the new array.
            for x in range(0, maze_size[0]):
                for y in range(0, maze_size[1]):
                    self.__squares[x][y] = MazeSquare(x, y, maze_size)

            start_loc = self.parse_line(fp.readline())
            finish_loc = self.parse_line(fp.readline())

            self.__start = self.__squares[start_loc[0]][start_loc[1]]
            self.__finish = self.__squares[finish_loc[0]][finish_loc[1]]

            for line in fp:
                legal_move = self.parse_line(line)
                self.__squares[legal_move[0]][legal_move[1]].add_move(
                    self.__squares[legal_move[2]][legal_move[3]])
            fp.close()

            for x in range(0, maze_size[0]):
                for y in range(0, maze_size[1]):
                    self.__squares[x][y].get_walls_Y()

            for y in range(0, maze_size[1]):
                for x in range(0, maze_size[0]):
                    self.__squares[x][y].get_walls_X()

            self.__drawing = self.make_drawing(maze_size, start_loc, finish_loc)

        except IOError:
            print("File '%s' not found." % filename)
            sys.exit(2)

    def get_start_square(self):
        return self.__start

    def is_finish_square(self, square):
        if square == self.__finish:
            return True
        else:
            return False

    def parse_line(self, line):
        return [int(i) for i in line.split()]

    def make_drawing(self, size, start, finish):
        allN = []
        allE = []
        allS = []
        allW = []
        for y in range(0, size[1]):
            for x in range(0, size[0]):
                walls = self.__squares[x][y].ret_walls()
                allN.append(walls[NORTH])
                allS.append(walls[SOUTH])

        for x in range(0, size[0]):
            for y in range(0, size[1]):
                walls = self.__squares[x][y].ret_walls()
                allE.append(walls[EAST])
                allW.append(walls[WEST])

        allN = [allN[3:6], allN[6:]]
        allS = [allS[:3], allS[3:6]]
        Y_LINE = [Y_STR, Y_STR, Y_STR]
        Y_All = [Y_LINE]
        for m in [0, 1]:
            buf = []
            for i in [0, 1, 2]:
                if allN[m][i] is not None:
                    buf.append(allN[m][i])
                elif allS[m][i] is not None:
                    buf.append(allS[m][i])
                else:
                    buf.append(Y_STR)
            Y_All.append(buf)
        Y_All.append(Y_LINE)

        allW = [allW[3:6], allW[6:]]
        allE = [allE[:3], allE[3:6]]
        X_LINE = [X_STR, X_STR, X_STR]
        X_All = [X_LINE]
        for m in [0, 1]:
            buf = []
            for i in [0, 1, 2]:
                if allW[m][i] is not None:
                    buf.append(allW[m][i])
                elif allE[m][i] is not None:
                    buf.append(allE[m][i])
                else:
                    buf.append(X_STR)
            X_All.append(buf)

        X_All.append(X_LINE)

        n = 0
        for i in range(0,4):
            # print("│%s─%s─%s│" % (Y_All[i][0], Y_All[i][1], Y_All[i][2]))
            print("│%s+%s+%s│" % (Y_All[i][0], Y_All[i][1], Y_All[i][2]))
            if n < 3:
                print("%s %s %s %s" %(X_All[0][n], X_All[1][n], X_All[2][n], X_All[3][n]))
            n += 1


class MazeSquare:
    """Represents one square in the maze."""
    def __init__(self, x, y, size):
        self.__moves = []
        self.__x = x
        self.__y = y
        self.__walls = [None, None, None, None]

    def add_move(self, neighbor):
        self.__moves.append(neighbor)

    def get_legal_moves(self):
        return self.__moves

    def get_location(self):
        return (self.__x, self.__y)

    def get_walls_Y(self):
        legal_moves = [i.get_location() for i in self.__moves]
        for move in legal_moves:
            if self.__y < move[1]:
                self.__walls[SOUTH] = Y_PAD
            elif self.__y > move[1]:
                self.__walls[NORTH] = Y_PAD

    def get_walls_X(self):
        legal_moves = [i.get_location() for i in self.__moves]
        for move in legal_moves:
            if self.__x < move[0]:
                self.__walls[EAST] = X_PAD
            elif self.__x > move[0]:
                self.__walls[WEST] = X_PAD

    def ret_walls(self):
        return self.__walls


##############################################################################
# FUNCTIONS


##############################################################################
# MAIN


def main(filename):
    print("\n-----   Attempting to solve maze in file: '%s'.   -----\n" % filename)
    is_solvable = True
    move_stack = Stack()
    maze = Maze(filename)
    square = maze.get_start_square()

    while not maze.is_finish_square(square) and is_solvable:
        for move in square.get_legal_moves():
            move_stack.push(move)
        if move_stack.is_empty():
            is_solvable = False
            # print("No legal moves remaining!")
        else:
            square = move_stack.pop()

    print()
    if is_solvable:
        print("Maze solved!")
    else:
        print("Maze cannot be solved")


##############################################################################

for filename in FILES:
    main(filename)
    print()
