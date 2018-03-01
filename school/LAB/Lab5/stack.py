#!/usr/bin/python3
import sys

FILE = "orig_input.txt"


###############################################################################
# Class Definitions

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

    def dbg_print(self):
        print(self.__items)


###############################################################################
# Function Definitions

def getpostfix(line):
    pfstack = Stack()
    output = []
    expr = line.strip().replace(' ', '')

    for ch in expr:
        if ch.isnumeric():
            output.append(str(ch))
        elif ch == '(':
            pfstack.push(ch)
        elif ch == ')':
            # Pop elements from the postix stack and append them to the output
            # until the previous opening parenthesis is found.
            tmp = pfstack.pop()
            while tmp != '(':
                if tmp != '(':
                    output.append(str(tmp))
                tmp = pfstack.pop()
        else:
            while (not pfstack.is_empty()) and (prec[pfstack.peek()] >= prec[ch]):
                output.append(str(pfstack.pop()))
            pfstack.push(ch)  # Always add the current operator to the stack

    while not pfstack.is_empty():
        output.append(str(pfstack.pop()))

    return output


def evaluatepostfix(postfix_expr):
    resultstack = Stack()
    buf = " ".join(postfix_expr)
    for token in postfix_expr:
        if token.isnumeric():
            resultstack.push(token)
        else:
            # NOTE: Following two lines exist only to make pylint stop whining
            # about the "unused" variables 'a' and 'b'.
            a = b = 0
            a += b
            b = float(resultstack.pop())
            a = float(resultstack.pop())
            resultstack.push(eval('a' + token + 'b'))
    print("%9.2f =  %s" % (float(resultstack.peek()), buf))


###############################################################################
# Main

def main(file_name):
    try:
        with open(file_name, 'r') as fp:
            for line in fp:
                postfix_expr = getpostfix(line)
                evaluatepostfix(postfix_expr)
    except IOError as errno:
        print("lrn2typ ->", errno)


prec = {'*': 3, '/': 3, '%': 3, '+': 2, '-': 2, '(': 1}
PRINT_INFIX = False
INIT = 1

if len(sys.argv) > 1:
    for filename in sys.argv[1:]:
        main(filename)
else:
    main(FILE)
