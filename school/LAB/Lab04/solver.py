#!/usr/bin/env python3
"""Please stop bugging me pylint this isn't even a module."""

import sys

FILES = ["solvable_maze.txt", "unsolvable_maze.txt", "4x4maze.txt", "6x6maze.txt"]
NORTH = 0
EAST = 1
SOUTH = 2
WEST = 3
X_STR = "|  "
Y_STR = "[1m---[0m"
X_PAD = "   "
Y_PAD = "   "


##############################################################################
# CLASSES


class Stack:
    def __init__(self):
        self.__items = []

    def push(self, item):
        """Push item to the stack."""
        self.__items.append(item)

    def pop(self):
        """Pop item from the stack."""
        return self.__items.pop()

    def is_empty(self):
        """Return whether the stack is empty."""
        return len(self.__items) == 0


class Maze:
    def __init__(self, file_name):
        self.__start = None
        self.__finish = None
        self.__error = None
        self.__squares = []
        try:
            fp = open(file_name, "r")
            maze_size = self.parse_line(fp.readline())
            if maze_size[0] != maze_size[1]:
                self.__error = "%s: Maze must be square!" % red("ERROR")
                return

            # Create the double array of zeroes. [x*[y*<0>]]
            for x in range(0, maze_size[0]):
                self.__squares.append(maze_size[1] * [0])

            # Make a MazeSquare object for every 0 in the new array.
            for x in range(0, maze_size[0]):
                for y in range(0, maze_size[1]):
                    self.__squares[x][y] = MazeSquare(x, y)

            # Get the start and finish and assign the MazeSquare objects at
            # those positions to the appropriate private attributes.
            start_loc = self.parse_line(fp.readline())
            finish_loc = self.parse_line(fp.readline())
            self.__start = self.__squares[start_loc[0]][start_loc[1]]
            self.__finish = self.__squares[finish_loc[0]][finish_loc[1]]

            # The rest of the file gives the legal moves of the maze in the
            # form [x1] [y1] [x2] [y2] where (x1,y1) is the starting square
            # and (x2,y2) is the destination. This loop parses the file and
            # assigns to the MazeSquare object at (x1,y1) the corresponding
            # destination squares.
            for line in fp:
                legal_move = self.parse_line(line)
                self.__squares[legal_move[0]][legal_move[1]].add_move(
                    self.__squares[legal_move[2]][legal_move[3]])
            fp.close()

            # These two loops
            for x in range(0, maze_size[0]):
                for y in range(0, maze_size[1]):
                    self.__squares[x][y].get_walls()

            self.__size = maze_size
            self.__maze_data = self.get_maze_data()
            self.__drawing = self.make_drawing()

        except IOError:
            self.__error = "%s: File '%s' not found." % (red("ERROR"), file_name)
            return

    def get_maze_data(self):
        """Abandon hope all ye who enter here."""
        data = [[], [], [], []]
        size = self.__size

        # NOTE: The next two sections COULD be functions, but it seems to me
        #       that there isn't really a good way to implement them as such
        #       without passing so many arbitrary variables as to make it
        #       unnecessarily confusing.
        # Get the north and south parts of the list of walls from each maze
        # square - ie those relating to the y-axis.
        for y1 in range(0, size[1]):
            temp1 = []
            temp2 = []
            for x1 in range(0, size[0]):
                walls = self.__squares[x1][y1].ret_walls()
                temp1.append(walls[NORTH])
                temp2.append(walls[SOUTH])
            data[NORTH].append(temp1)
            data[SOUTH].append(temp2)

        # Get the east and west parts of the list of walls from each maze
        # square - ie those relating to the x-axis.
        for x1 in range(0, size[0]):
            temp1 = []
            temp2 = []
            for y1 in range(0, size[1]):
                walls = self.__squares[x1][y1].ret_walls()
                temp1.append(walls[EAST])
                temp2.append(walls[WEST])
            data[EAST].append(temp1)
            data[WEST].append(temp2)

        # Split off the first and last list from north and south respectively,
        # and west and east respectively - these are always walls because the
        # maze is fully surrounded so they don't need to be analyzed.
        data[NORTH] = data[NORTH][1:]
        data[SOUTH] = data[SOUTH][:-1]
        data[WEST] = data[WEST][1:]
        data[EAST] = data[EAST][:-1]

        Y_All = self.analyze_wall_data(data[NORTH], data[SOUTH], Y_STR, size[1])
        X_All = self.analyze_wall_data(data[WEST], data[EAST], X_STR, size[0])
        return [X_All, Y_All]

    def analyze_wall_data(self, dir1, dir2, STRING, size):
        """NOTE: This method was originally implemented 'in-line' for x and y
        rather than as a method. I am not actually sure whether it is an
        improvement for it to be one or if it might be better in this case to
        duplicate the code and not pass so many variables around.
        """
        # LINE is a full line of whatever STRING constant is applicable,
        # representing the filled outer edges of the maze. One is added to the
        # beginning and the end of the data.
        LINE = [STRING]*size
        data = [LINE]

        # Check whether any data has been recorded for each section of the
        # potential inner walls of the maze. If nothing is found then we
        # must assume there is a wall since all data is obtained from
        # analyzing legal moves which are only possible when no wall between
        # two squares exists. In that case, add the predefined 'wall' STRING
        # for the given axis' walls, otherwise just add the predeternimed
        # 'space' STRING for that position.
        for x in range(0, len(dir1)):
            buf = []
            for y in range(0, len(dir1[x])):
                if dir1[x][y] is not None:
                    buf.append(dir1[x][y])
                elif dir2[x][y] is not None:
                    buf.append(dir2[x][y])
                else:
                    buf.append(STRING)
            if len(buf) < size:
                buf.append(STRING)
            data.append(buf)
        data.append(LINE)
        return data

    def make_drawing(self, cur_square=None):
        """This method is just split off from the get_maze_data method.
        Similar advice applies.
        """
        X_All = self.__maze_data[0]
        Y_All = self.__maze_data[1]
        size = self.__size

        if cur_square is None:
            cur_square = self.__start

        # This section is so hideous that the thought of commenting it makes
        # me physically ill.
        drawing = ""

        Y_drawing = []
        for x in range(0, len(Y_All)):
            buf = "+"
            for y in range(0, len(Y_All[x])):
                buf += "%s+" % Y_All[x][y]
            if buf != " ":
                Y_drawing.append("%s \n" % buf)

        y = 0
        while y < len(Y_drawing):
            if y <= size[1]:
                drawing += Y_drawing[y]
            try:
                for x in range(0, len(X_All)):
                    if x < size[0] and y < size[1]:
                        temp1 = X_All[x][y]
                        if self.__squares[x][y] == cur_square:
                            if self.__squares[x][y] == self.__start:
                                temp1 = temp1[:-1] + green("S")
                            elif self.__squares[x][y] == self.__finish:
                                temp1 = temp1[:-1] + green("F")
                            else:
                                temp1 = temp1[:-1] + green("*")
                        else:
                            if self.__squares[x][y] == self.__start:
                                temp1 = temp1[:-1] + bold("S")
                            elif self.__squares[x][y] == self.__finish:
                                temp1 = temp1[:-1] + bold("F")
                        drawing += temp1 + " "
                    else:
                        drawing += X_All[x][y] + " "

                drawing += '\n'
                y += 1
            except IndexError:
                break

        return drawing

    def write_drawing(self, square=None):
        """Print the maze drawing."""
        if square is not None:
            self.__drawing = self.make_drawing(square)
        print(self.__drawing, end='')

    def parse_line(self, line):
        """Used by the constructor to parse files."""
        return [int(i) for i in line.split()]

    def get_start_square(self):
        """Return starting square"""
        return self.__start

    def is_finish_square(self, square):
        """Return whether given square is the finish square."""
        return square == self.__finish

    def check_error(self):
        if self.__error is not None:
            print(self.__error)
            return True
        else:
            return False

    def get_finish_location(self):
        return self.__finish.get_location()


class MazeSquare:
    def __init__(self, x, y):
        self.__moves = []
        self.__x = x
        self.__y = y
        self.__walls = [None, None, None, None]  # North, East, South, West

    def add_move(self, neighbor):
        """Add a legal move to the square's list (used in setup)."""
        self.__moves.append(neighbor)

    def get_legal_moves(self):
        """Return the full list of legal moves from this square."""
        return self.__moves

    def get_location(self):
        """Return the (x,y) coordinates of the square's location."""
        return (self.__x, self.__y)

    def get_walls(self):
        """Create the messy walls tuple."""
        legal_moves = [i.get_location() for i in self.__moves]
        for move in legal_moves:
            if self.__y > move[1]:
                self.__walls[NORTH] = Y_PAD
            elif self.__y < move[1]:
                self.__walls[SOUTH] = Y_PAD

            if self.__x < move[0]:
                self.__walls[EAST] = X_PAD
            elif self.__x > move[0]:
                self.__walls[WEST] = X_PAD

    def ret_walls(self):
        """Return the walls tuple."""
        return self.__walls


##############################################################################
# FUNCTIONS


def bold(string):
    return "[1m" + string + "[0m"

def red(string):
    return "[1m[31m" + string + "[0m"

def green(string):
    return "[1m[32m" + string + "[0m"

def yellow(string):
    return "[0m[33m" + string + "[0m"

def magenta(string):
    return "[0m[35m" + string + "[0m"


def solve(file_name):
    """Solve and display a maze in a given file."""
    is_solvable = True
    move_stack = Stack()

    print(bold("%s\n%s\n" % ("="*60, "="*60)))
    print(yellow("Attempting to solve maze in file: '") + magenta(file_name) +
          yellow("'.\n"))

    maze = Maze(file_name)
    if maze.check_error():
        return -1
    print(bold("STARTING POSITION"))
    maze.write_drawing()
    square = maze.get_start_square()

    i = 1
    while not maze.is_finish_square(square) and is_solvable:
        for move in square.get_legal_moves():
            move_stack.push(move)
        if move_stack.is_empty():
            is_solvable = False
        else:
            square = move_stack.pop()
        print(bold("<MOVE %d>" % i))
        maze.write_drawing(square)
        i += 1

    if is_solvable:
        print(green("\nMaze is solvable."))
    else:
        print(red("\nMaze cannot be solved."))


##############################################################################
# MAIN


for filename in FILES:
    solve(filename)
    print()
if len(sys.argv) > 1:
    for arg in sys.argv[1:]:
        solve(arg)
        print()
