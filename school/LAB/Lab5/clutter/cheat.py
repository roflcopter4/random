#!/usr/bin/python3
import sys


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


def getpostfix(line):
    pfstack = Stack()
    output = []
    expr = line.strip().replace(' ', '')  # gettin rid of the spaces

    for ch in expr:
        if ch.isnumeric():
            output.append(str(ch))
        elif ch == '(':
            pfstack.push(ch)
        elif ch == ')':
            ppd = None
            while ppd != '(':
                if ppd is not None and ppd != '(':
                    output.append(str(ppd))
                ppd = pfstack.pop()
        else:
            while (not pfstack.is_empty()) and (prec[pfstack.peek()] >= prec[ch]):
                output.append(str(pfstack.pop()))
            pfstack.push(ch)

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
            # if token == '%':
                # b = int(resultstack.pop())
                # a = int(resultstack.pop())
                # resultstack.push(float(eval('a'+token+'b')))
            # else:
            b = float(resultstack.pop())
            a = float(resultstack.pop())
            resultstack.push(eval('a'+token+'b'))
    # print('Result:  %9.2f' % float(resultstack.peek()))
    print("%9.2f =  %s" % (float(resultstack.peek()), buf))


prec = {'*': 3, '/': 3, '%': 3, '+': 2, '-': 2, '(': 1}
if len(sys.argv) > 1:
    input_file_name = sys.argv[1]
else:
    input_file_name = 'orig_input.txt'

try:
    with open(input_file_name, 'r') as fp:
        for line in fp:
            postfix_expr = getpostfix(line)
            evaluatepostfix(postfix_expr)
except IOError:
    print("Moron")
