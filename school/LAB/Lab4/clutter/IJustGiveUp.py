#!/usr/bin/env python3
"""Please stop bugging me pylint this isn't even a module."""

import sys

# FILES = ["solvable_maze.txt", "unsolvable_maze.txt"]
FILES = ["solvable_maze.txt", "unsolvable_maze.txt", "bastymaze.txt", "unsolvable_maze2.txt"]
NORTH = 0
EAST = 1
SOUTH = 2
WEST = 3
X_STR = "|  "
Y_STR = "---"
X_PAD = "   "
Y_PAD = "   "

##############################################################################
# CLASSES


class Stack:
    """Simple representation of a stack."""

    def __init__(self):
        self.__items = []

    def push(self, item):
        """Push item to the stack."""
        self.__items.append(item)

    def pop(self):
        """Pop item from the stack."""
        return self.__items.pop()

    def peek(self):
        """Return last item from the stack without removing it."""
        return self.__items[-1]

    def is_empty(self):
        """Return whether the stack is empty."""
        return len(self.__items) == 0

    def size(self):
        """Return the size of the stack."""
        return len(self.__items)

    def clear(self):
        """Clear the stack."""
        self.__items = []


class Maze:
    """Constructs and represents a whole maze, given a filename."""

    def __init__(self, file_name):
        self.__start = None
        self.__finish = None
        self.__squares = []
        try:
            fp = open(file_name, "r")
            maze_size = self.parse_line(fp.readline())

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
            self.__drawing = self.make_drawing()

        except IOError:
            print("File '%s' not found." % file_name)
            sys.exit(2)

    def make_drawing(self):
        """Abandon all hope ye who enter here."""
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

        # This section is so hideous that the thought of commenting it makes
        # me physically anxious.
        drawing = ""

        Y_drawing = []
        for x in range(0, len(Y_All)):
            buf = " "
            for y in range(0, len(Y_All[x])):
                buf += "%s+" % Y_All[x][y]
                # if y < size[1]-1:
                    # buf += '+'
            if buf != " ":
                Y_drawing.append("%s \n" % buf)

        # print(Y_drawing)

        X_drawing = []
        for x in range(0, len(X_All)):
            buf = ""
            for y in range(0, len(X_All[x])):
                buf += "%s " % X_All[x][y]
            if buf != "":
                X_drawing.append("%s\n" % buf)

        # print(X_drawing)

        if len(X_All) < len(Y_All):
            temp1 = len(X_All[0])
            diff = len(Y_All) - len(X_All)
            for i in range(0, diff):
                # X_All[i].append("|  ")
                X_All.append([X_STR]*(temp1+1))
        elif len(X_All) > len(Y_All):
            temp1 = len(Y_All[0])
            diff = len(X_All) - len(Y_All)
            for i in range(0, diff):
                Y_All.append([Y_STR]*temp1)


        # for x in range(0, len(Y_All)):
            # buf = " "
            # for y in range(0, len(Y_All[x])):
                # buf += "%s" % Y_All[x][y]
                # if y < size[1]-1:
                    # buf += "+"
                # buf += '\n'
                # buf += "%s" % X_All[y][x]

                # # print(buf)
            # drawing += "%s \n" % buf
            # print(drawing)



        if len(X_drawing) < len(Y_drawing):
            temp1 = len(X_drawing[0])
            diff = len(Y_drawing) - len(X_drawing)
            for i in range(0, diff):
                X_drawing.append("")
        elif len(X_drawing) > len(Y_drawing):
            temp1 = len(Y_drawing[0])
            diff = len(X_drawing) - len(Y_drawing)
            for i in range(0, diff):
                Y_drawing.append("")

        n = 0
        x = 0
        # for x in range(0, len(Y_drawing)):
        print(X_All)
        while x < len(Y_drawing):
            if x <= size[1]:
                drawing += Y_drawing[x]

            try:
                for n in range(0, len(X_All)):
                    drawing += X_All[n][x] + " "
                    print(drawing + "\n\n\n")

                    # drawing += X_All[0][x] + " "
                    # print(drawing + "\n\n\n")

                    # drawing += X_All[1][x] + " "
                    # print(drawing + "\n\n\n")

                    # drawing += X_All[2][x] + " "
                    # print(drawing + "\n\n\n")

                    # drawing += X_All[3][x] + " \n"
                    # print(drawing + "\n\n\n")
                drawing += '\n'
                x += 1
            except IndexError:
                break

        # print(drawing)


        # y2 = 0
        # for x1 in range(0, size[1])+1):
            # # The y-axis portion
            # buf = "│"
            # for y1 in range(0, size[1]):
                # buf += "%s" % Y_All[x1][y1]
                # if y1 < size[1]-1:
                    # buf += "+"
            # drawing += "%s│\n" % buf

            # # The x-axis portion
            # buf = ""
            # if y2 < size[0]:
                # for x2 in range(0, size[0]+1):
                    # temp1 = X_All[x2][y2]
                    # if x2 < size[0]:
                        # if self.__squares[x2][y2] == self.__start:
                            # temp1 = temp1[:-1] + 'S'
                        # elif self.__squares[x2][y2] == self.__finish:
                            # temp1 = temp1[:-1] + 'F'
                    # buf += "%s " % temp1
                # drawing += "%s\n" % buf
                # y2 += 1

        # y2 = 0
        # for x1 in range(0, size[1]):
            # # The y-axis portion
            # buf = "│"
            # for y1 in range(0, len(Y_All[x1])):
                # buf += "%s" % Y_All[x1][y1]
                # if y1 < size[1]-1:
                    # buf += "+"
            # drawing += "%s│\n" % buf

            # # The x-axis portion
        # buf = ""
        # if y2 < size[0]:
            # for x2 in range(0, size[0]+1):
                # temp1 = X_All[x2][y2]
                # if x2 < size[0]:
                    # if self.__squares[x2][y2] == self.__start:
                        # temp1 = temp1[:-1] + 'S'
                    # elif self.__squares[x2][y2] == self.__finish:
                        # temp1 = temp1[:-1] + 'F'
                # buf += "%s " % temp1
            # drawing += "%s\n" % buf
            # y2 += 1

        return drawing

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
        # print(dir1)
        # print(dir2)

        # temp1 = []
        # for x in dir1:
            # buf = []
            # for y in x:
                # if y is not None:
                    # buf.append(y)
            # temp1.append(buf)
        # temp1.append(LINE)
        # print(temp1)

        temp1 = [LINE]
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
            temp1.append(buf)
        temp1.append(LINE)
        # print(temp1)
        return temp1

        # temp2 = []
        # for x in dir2:
            # buf = []
            # for y in x:
                # if y is not None:
                    # buf.append(y)
            # temp2.append(buf)
        # temp2.append(LINE)
        # print(temp2)


        for x in range(0, 2):
            buf = []
            for y in range(0, size):
                if dir1[x][y] is not None:
                    buf.append(dir1[x][y])
                elif dir2[x][y] is not None:
                    buf.append(dir2[x][y])
                else:
                    buf.append(STRING)
            data.append(buf)
        data.append(LINE)

        # print(data)
        return data

    def parse_line(self, line):
        """Used by the constructor to parse files."""
        return [int(i) for i in line.split()]

    def get_start_square(self):
        """Return starting square"""
        return self.__start

    def is_finish_square(self, square):
        """Return whether given square is the finish square."""
        return square == self.__finish

    def write_drawing(self):
        """Print the maze drawing."""
        print(self.__drawing)


class MazeSquare:
    """Represents one square in the maze."""

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


def solve(file_name):
    """Solve and display a maze in a given file."""
    is_solvable = True
    move_stack = Stack()
    maze = Maze(file_name)
    square = maze.get_start_square()

    print("-"*60)
    print("Attempting to solve maze in file: '%s'.\n" % file_name)
    maze.write_drawing()

    while not maze.is_finish_square(square) and is_solvable:
        for move in square.get_legal_moves():
            move_stack.push(move)
        if move_stack.is_empty():
            is_solvable = False
            # print("No legal moves remaining!")
        else:
            square = move_stack.pop()

    if is_solvable:
        print("Maze is solvable.")
    else:
        print("Maze cannot be solved.")


##############################################################################
# MAIN


for filename in FILES:
    solve(filename)
    print()
