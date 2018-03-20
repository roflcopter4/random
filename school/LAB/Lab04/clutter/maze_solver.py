#!/usr/bin/env python3

FILES = ["solvable_maze.txt", "unsolvable_maze.txt"]

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
        try:
            fp = open(filename, "r")
            maze_size = parse_line(fp.readline())
            self.__squares = []

            for x in range(0, maze_size[0]):
                self.__squares.append([0] * maze_size[1])

            x = 0
            while x < maze_size[0]:
                y = 0
                while y < maze_size[1]:
                    square = MazeSquare(x, y)
                    self.__squares[x][y] = square
                    y += 1
                x += 1

            start_loc = parse_line(fp.readline())
            self.__start = self.__squares[start_loc[0]][start_loc[1]]

            finish_loc = parse_line(fp.readline())
            self.__finish = self.__squares[finish_loc[0]][finish_loc[1]]

            for line in fp:
                legal_move = parse_line(line)
                self.__squares[legal_move[0]][legal_move[1]].add_move(
                    self.__squares[legal_move[2]][legal_move[3]])
            fp.close()
        except IOError:
            print("File '%s' not found." % filename)

    def get_start_square(self):
        return self.__start

    def is_finish_square(self, square):
        if square == self.__finish:
            return True
        else:
            return False


class MazeSquare:
    """Represents one square in the maze."""
    def __init__(self, x, y):
        self.__moves = []
        self.__x = x
        self.__y = y

    def add_move(self, neighbor):
        self.__moves.append(neighbor)

    def get_legal_moves(self):
        return self.__moves

    def get_location(self):
        return (self.__x, self.__y)


##############################################################################
# FUNCTIONS


def parse_line(line):
    """This function takes a string as input, and returns a list of the ints
    in the string.  This is used by the Maze constructor.
    """
    line_contents = line.split()
    i = 0
    while i < len(line_contents):
        line_contents[i] = int(line_contents[i])
        i += 1
    return line_contents


##############################################################################
# MAIN


def main(filename):
    is_solvable = True
    move_stack = Stack()
    maze = Maze(filename)
    square = maze.get_start_square()

    print("-----   Attempting to solve maze in file: '%s'.   -----" % filename)

    while not maze.is_finish_square(square) and is_solvable:
        for move in square.get_legal_moves():
            move_stack.push(move)
        if move_stack.is_empty():
            is_solvable = False
            print("No legal moves remaining!")
        else:
            square = move_stack.pop()

    if is_solvable:
        print("Maze solved!")
    else:
        print("Maze cannot be solved")


##############################################################################

for filename in FILES:
    main(filename)
    print()
