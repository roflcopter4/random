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


class Maze:
    """Constructs and represents a whole maze, given a filename."""

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
                    self.__squares[x][y] = MazeSquare(x, y)

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

            self.__size = maze_size
            self.__drawing = self.make_drawing()

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
        """Used by the constructor to parse files."""
        return [int(i) for i in line.split()]

    def make_drawing(self):
        """Abandon all hope ye who enter here."""
        allNorth = []
        allEast = []
        allSouth = []
        allWest = []
        size = self.__size

        # Get the north and south parts of the list of walls from each maze
        # square - ie those relating to the y-axis.
        for y in range(0, size[1]):
            temp1 = []
            temp2 = []
            for x in range(0, size[0]):
                walls = self.__squares[x][y].ret_walls()
                temp1.append(walls[NORTH])
                temp2.append(walls[SOUTH])
            allNorth.append(temp1)
            allSouth.append(temp2)

        # Split off the first and last list from north and south respectively -
        # these are always walls because the maze is fully surrounded.
        allNorth = allNorth[1:]
        allSouth = allSouth[:-1]
        Y_LINE = [Y_STR]*size[1]
        Y_All = [Y_LINE]

        # Check whether any data has been recorded for each section of the
        # potential inner walls of the maze. If nothing is found then we
        # must assume there is a wall since all data is obtained from
        # analyzing legal moves which are only possible when no wall between
        # two squares exists. In that case, add the predefined 'wall' string
        # for the y-axis walls, otherwise just add the predeternimed 'space'
        # string for that position.
        for m in range(0, len(Y_All)+1):
            buf = []
            for i in range(0, size[1]):
                if allNorth[m][i] is not None:
                    buf.append(allNorth[m][i])
                elif allSouth[m][i] is not None:
                    buf.append(allSouth[m][i])
                else:
                    buf.append(Y_STR)
            Y_All.append(buf)
        Y_All.append(Y_LINE)

        # Get the east and west parts of the list of walls from each maze
        # square - ie those relating to the x-axis.
        for x in range(0, size[0]):
            temp1 = []
            temp2 = []
            for y in range(0, size[1]):
                walls = self.__squares[x][y].ret_walls()
                temp1.append(walls[EAST])
                temp2.append(walls[WEST])
            allEast.append(temp1)
            allWest.append(temp2)

        # Strip east and west for the same reason as north and south.
        allWest = allWest[1:]
        allEast = allEast[:-1]

        X_All = self.analyze_wall_data(allWest, allEast, X_STR, size[0])

        # X_LINE = [X_STR]*size[0]
        # X_All = [X_LINE]

        # # The same procedure as above but for the x-axis walls.
        # for m in range(0, len(X_All)+1):
        #     buf = []
        #     for i in range(0, size[0]):
        #         if allWest[m][i] is not None:
        #             buf.append(allWest[m][i])
        #         elif allEast[m][i] is not None:
        #             buf.append(allEast[m][i])
        #         else:
        #             buf.append(X_STR)
        #     X_All.append(buf)
        # X_All.append(X_LINE)

        drawing = ""
        n = 0
        for a in range(0, size[1]+1):
            buf = "│"
            for b in range(0, size[1]):
                buf += "%s" % Y_All[a][b]
                if b < size[1]-1:
                    buf += "+"
            drawing += buf + "│\n"

            buf = ""
            if n < size[0]:
                for m in range(0, size[0]+1):
                    buf += "%s " % X_All[m][n]
                drawing += buf + "\n"
                n += 1

        return drawing

    def write_drawing(self):
        print(self.__drawing)

    def analyze_wall_data(self, dir1, dir2, string, size):
        """NOTE: This method was originally implemented 'in-line' for x and y
        rather than as a method. I am not actually sure whether it is an
        improvement for it to be one or if it might be better in this case to
        duplicate the code and not pass so many variables around.
        """
        # Check whether any data has been recorded for each section of the
        # potential inner walls of the maze. If nothing is found then we
        # must assume there is a wall since all data is obtained from
        # analyzing legal moves which are only possible when no wall between
        # two squares exists. In that case, add the predefined 'wall' string
        # for the given axis' walls, otherwise just add the predeternimed
        # 'space' string for that position.

        LINE = [string]*size[0]
        data = [LINE]
        for m in range(0, len(data)+1):
            buf = []
            for i in range(0, size):
                if dir1[m][i] is not None:
                    buf.append(dir1[m][i])
                elif dir2[m][i] is not None:
                    buf.append(dir2[m][i])
                else:
                    buf.append(string)
            data.append(buf)
        data.append(LINE)

        return data



class MazeSquare:
    """Represents one square in the maze."""

    def __init__(self, x, y):
        self.__moves = []
        self.__x = x
        self.__y = y
        self.__walls = [None, None, None, None]  # North, East, South, West

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
    is_solvable = True
    move_stack = Stack()
    maze = Maze(filename)
    square = maze.get_start_square()

    print("\nAttempting to solve maze in file: '%s'.\n" % filename)
    maze.write_drawing()

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
