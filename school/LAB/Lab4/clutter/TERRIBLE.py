


import sys
import os, re

os.system('echo hi')

FILES = ["solvable_maze.txt", "unsolvable_maze.txt", "4x4maze.txt"]
N = 0
E = 1
S = 2
W = 3
X_STR = "|  "
Y_STR = "---"
X_PAD = "   "
Y_PAD = "   "





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

    def s(self):

        return len(self.__items)

    def clear(self):

        self.__items = []
                                                                                                                                                                                    

1
2
3
4
5
6
7
2 + 3
a = 3
a
a
a
a
a

class Maze:












    def __init__(self, file_name):
        self.__start = None
        self.__finish = None
        self.__squares = []
        try:
            fp = open(file_name, "r")
            maze_s = self.parse_line(fp.readline())
            if maze_s[0] != maze_s[1]:
                print("ERROR: Maze must be square. Don't know how to return a null object so I'm killing the program.")
                sys.exit()

            
            for x in range(0, maze_s[0]):
                self.__squares.append(maze_s[1] * [0])

            
            for x in range(0, maze_s[0]):
                for y in range(0, maze_s[1]):
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

            
            for x in range(0, maze_s[0]):
                for y in range(0, maze_s[1]):
                    self.__squares[x][y].get_w()

            self.__s = maze_s
            self.__d = self.make_d()

        except IOError:
            print("File '%s' not found." % file_name)
            sys.exit(2)

    def make_d(self):

        d = [[], [], [], []]
        s = self.__s

        
        
        
        

        
        
        for y1 in range(0, s[1]):
            t = []
            l = []
            for x1 in range(0, s[0]):
                w = self.__squares[x1][y1].ret_w()
                t.append(w[N])
                l.append(w[S])
            d[N].append(t)
            d[S].append(l)

        
        
        for x1 in range(0, s[0]):
            t = []
            l = []
            for y1 in range(0, s[1]):
                w = self.__squares[x1][y1].ret_w()
                t.append(w[E])
                l.append(w[W])
            d[E].append(t)
            d[W].append(l)

        
        
        
        d[N] = d[N][1:]
        d[S] = d[S][:-1]
        d[W] = d[W][1:]
        d[E] = d[E][:-1]

        Y = self.an(d[N], d[S], Y_STR, s[1])
        X = self.an(d[W], d[E], X_STR, s[0])

        
        
        d = ""

        Y_d = []
        for x in range(0, len(Y)):
            buf = "+"
            for y in range(0, len(Y[x])):
                buf += "%s+" % Y[x][y]
            if buf != " ":
                Y_d.append("%s \n" % buf)

        n = 0
        x = 0
        while x < len(Y_d):
            if x <= s[1]:
                d += Y_d[x]

            try:
                for n in range(0, len(X)):
                    if n < s[0] and x < s[1]:
                        t = X[n][x]
                        if self.__squares[n][x] == self.__start:
                            t = t[:-1] + 'S'
                        elif self.__squares[n][x] == self.__finish:
                            t = t[:-1] + 'F'
                        d += t + " "
                    else:
                        d += X[n][x] + " "

                d += '\n'
                x += 1
            except IndexError:
                break
        if False:
            if False:
                if False:
                    if False:
                        1
                    else:
                        2
                elif False:
                    3
        return d

    def an(self, a, b, STRING, s):
        
        
        
        LINE = [STRING]*s
        d = [LINE]

        
        
        
        
        
        
        

        for x in range(0, len(a)):
            buf = []
            for y in range(0, len(a[x])):
                if a[x][y] is not None:
                    buf.append(a[x][y])
                elif b[x][y] is not None:
                    buf.append(b[x][y])
                else:
                    buf.append(STRING)
            if len(buf) < s:
                buf.append(STRING)
            d.append(buf)
        d.append(LINE)
        return d

    def parse_line(self, line):

        return [int(i) for i in line.split()]

    def get_start_square(self):

        return self.__start

    def is_finish_square(self, square):

        return square == self.__finish

    def write_d(self):

        print(self.__d)

3
class MazeSquare:


    def __init__(self, x, y):
        self.__moves = []
        self.__x = x
        self.__y = y
        self.__w = [None, None, None, None]  

    def add_move(self, neighbor):

        self.__moves.append(neighbor)

    def get_legal_moves(self):

        return self.__moves

    def get_location(self):

        return (self.__x, self.__y)

    def get_w(self):

        legal_moves = [i.get_location() for i in self.__moves]
        for move in legal_moves:
            if self.__y > move[1]:
                self.__w[N] = Y_PAD
            elif self.__y < move[1]:
                self.__w[S] = Y_PAD

            if self.__x < move[0]:
                self.__w[E] = X_PAD
            elif self.__x > move[0]:
                self.__w[W] = X_PAD

    def ret_w(self):

        return self.__w






def solve(file_name):

    is_solvable = True
    move_stack = Stack()
    maze = Maze(file_name)
    if maze == -1:
        return -1
    square = maze.get_start_square()

    print("-"*60)
    print("Attempting to solve maze in file: '%s'.\n" % file_name)
    maze.write_d()

    while not maze.is_finish_square(square) and is_solvable:
        for move in square.get_legal_moves():
            move_stack.push(move)
        if move_stack.is_empty():
            is_solvable = False
            
        else:
            square = move_stack.pop()

    if is_solvable:
        print("Maze is solvable.")
    else:
        print("Maze cannot be solved.")





while 1 > 2:
    print("HI")




for filename in FILES:
    solve(filename)
    print()
