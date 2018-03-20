#!/usr/bin/env python3

import sys

FILES = ["solvable_maze.txt", "unsolvable_maze.txt"]
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
                    self.__squares[x][y].get_walls()

            for y in range(0, maze_size[1]):
                for x in range(0, maze_size[0]):
                    self.__squares[x][y].get_walls_X()

            self.__drawing = self.make_drawing(maze_size, start_loc,
                                               finish_loc)

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
        X_Walls = [[None]*size[1]]*(size[0]+1)
        Y_Walls = [[None]*size[0]]*(size[1]+1)

        # for y in range(0, size[0]):
            # for x in range(0, size[1]):
                # walls = self.__squares[x][y].ret_walls()

                # if X_Walls[x][y] is None:
                    # if walls[WEST] is not None:
                        # X_Walls[x][y] = walls[WEST]

                # if X_Walls[x+1][y] is None:
                    # if walls[EAST] is not None:
                        # X_Walls[x+1][y] = walls[EAST]

        allN = []
        allE = []
        allS = []
        allW = []
        for y in range(0, size[1]):
            for x in range(0, size[0]):
                walls = self.__squares[x][y].ret_walls()
                # print(walls)
                allN.append(walls[NORTH])
                # allE.append(walls[EAST])
                allS.append(walls[SOUTH])
                # allW.append(walls[WEST])
                # if X_Walls[x][y] is None:
                    # if walls[WEST] is not None:
                        # X_Walls[x][y] = walls[WEST]

                # if Y_Walls[x][y] is None:
                    # if walls[NORTH] is not None:
                        # Y_Walls[x][y] = walls[NORTH]

                # if Y_Walls[x+1][y] is None:
                    # if walls[SOUTH] is not None:
                        # Y_Walls[x+1][y] = walls[SOUTH]


        for x in range(0, size[0]):
            for y in range(0, size[1]):
                walls = self.__squares[x][y].ret_walls()
                allE.append(walls[EAST])
                allW.append(walls[WEST])

        # print(allN)
        # print(allS)
        # print(allE)
        # print(allW)

        allN = [allN[3:6], allN[6:]]
        allS = [allS[:3], allS[3:6]]
        # print(allN)
        # print(allS)

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

        # print(Y_All)


        allW = [allW[3:6], allW[6:]]
        allE = [allE[:3], allE[3:6]]
        # print(allE)
        # print(allW)

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
                    # buf.append(' ')
            X_All.append(buf)

        X_All.append(X_LINE)

        # print(X_All)

        n = 0
        for i in range(0,4):
            print("│%s─%s─%s│" % (Y_All[i][0], Y_All[i][1], Y_All[i][2]))
            if n < 3:
                print("%s %s %s %s" %(X_All[0][n], X_All[1][n], X_All[2][n], X_All[3][n]))
            n += 1

        # for buf in X_All:
            # print("|%s%s%s|" %(buf[0], buf[1], buf[2]))


        # print(X_Walls)
        # print(Y_Walls)

        # X_Walls = [['-', '-', '-'], [None*3], [None*3], ['-', '-', '-']]
        # X_Walls = [['-', '-', '-']*4]
        # Y_Walls = [['|', '|', '|']*4]
        # grid = []
        # for x in range(0, size[0]):
            # grid.append(size[1] * [0])

        # X_Walls = [[None]*size[1]]*(size[0]+1)
        # X_Walls[0] = [True*size[1]]
        # X_Walls[size[0]+1] = [True*size[1]]

        # Y_Walls = [[None]*size[0]]*(size[1]+1)
        # Y_Walls[0] = [True*size[0]]
        # Y_Walls[size[1]+1] = [True*size[0]]

        # print(X_Walls)
        # print(Y_Walls)

        # for x in range(0, size[0]):
            # for y in range(0, size[1]):
                # # grid[x][y] = self.get_grid_walls(size, x, y)
                # # grsquare = grid[x][y]
                # grsquare = self.get_grid_walls(size, x, y)
                # print(grsquare)

                # if grsquare[EAST] is not None:
                    # if grsquare[EAST]:
                        # X_Walls[x+1][y] = '-'
                    # else:
                        # X_Walls[x+1][y] = False

                # if grsquare[WEST] is not None:
                    # if grsquare[WEST]:
                        # X_Walls[x][y] = '-'
                    # else:
                        # X_Walls[x][y] = False

                # if grsquare[NORTH] is not None:
                    # if grsquare[NORTH]:
                        # Y_Walls[x][y] = '|'
                    # else:
                        # Y_Walls[x][y] = False

                # if grsquare[SOUTH] is not None:
                    # if grsquare[SOUTH]:
                        # Y_Walls[x+1][y] = '|'
                    # else:
                        # Y_Walls[x+1][y] = False

        # print(X_Walls)
        # print(Y_Walls)


    # def get_grid_walls(self, size, x1, y1):
        # walls = [None, None, None, None]  # North, East, South, West

        # if x1 == 0:
            # walls[WEST] = True
        # elif x1 == size[0]-1:
            # walls[EAST] = True

        # if y1 == 0:
            # walls[NORTH] = True
        # elif y1 == size[1]-1:
            # walls[SOUTH] = True

        # legal_moves = [i.get_location() for i in self.__squares[x1][y1].get_legal_moves()]
        # if len(legal_moves) > 0:
            # for move in legal_moves:
                # print(move)
                # x2 = move[0]
                # y2 = move[1]
                # if x1 < x2:
                    # walls[EAST] = False
                # elif x1 > x2:
                    # walls[WEST] = False
                # # else:
                    # # walls[EAST] = True
                    # # walls[WEST] = True

                # if y1 > y2:
                    # walls[NORTH] = False
                # elif y1 < y2:
                    # walls[SOUTH] = False
                # # else:
                    # # walls[NORTH] = True
                    # # walls[SOUTH] = True

        # print(x1, y1, walls)
        # return walls


class MazeSquare:
    """Represents one square in the maze."""
    def __init__(self, x, y, size):
        self.__moves = []
        self.__x = x
        self.__y = y
        self.__walls = [None, None, None, None]
        # if x == 0:
            # self.__walls[WEST] = '|'
        # elif x == (size[0]-1):
            # self.__walls[EAST] = '|'
        # if y == 0:
            # self.__walls[NORTH] = '-'
        # elif y == size[1]-1:
            # self.__walls[SOUTH] = '-'

    def add_move(self, neighbor):
        self.__moves.append(neighbor)

    def get_legal_moves(self):
        return self.__moves

    def get_location(self):
        return (self.__x, self.__y)

    def get_walls(self):
        legal_moves = [i.get_location() for i in self.__moves]
        for move in legal_moves:
            # print(self.__x, self.__y, move)
            # if self.__x < move[0] and self.__walls[EAST] is None:
                # self.__walls[EAST] = ' '
            # elif self.__x > move[0] and self.__walls[WEST] is None:
                # self.__walls[WEST] = ' '

            if self.__y < move[1] and self.__walls[SOUTH] is None:
                self.__walls[SOUTH] = Y_PAD
            elif self.__y > move[1] and self.__walls[NORTH] is None:
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

# class RealMaze:
    # def __init__(self):



##############################################################################
# FUNCTIONS


# def parse_line(line):
    # line_contents = line.split()
    # i = 0
    # while i < len(line_contents):
        # line_contents[i] = int(line_contents[i])
        # i += 1
    # return line_contents


##############################################################################
# MAIN


def main(filename):
    print("\n-----   Attempting to solve maze in file: '%s'.   -----\n" % filename)
    is_solvable = True
    move_stack = Stack()
    maze = Maze(filename)
    square = maze.get_start_square()

    # print("-----   Attempting to solve maze in file: '%s'.   -----" % filename)

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
