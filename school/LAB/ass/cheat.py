#!/usr/bin/env python3
import sys


class Stack:

    def __init__(self):
        self.__items = []

    def push(self, item):
        self.__items.append(item)

    def pop(self):
        return self.__items.pop()

    def peek(self):
        return self.__items[len(self.__items)-1]

    def is_empty(self):
        return len(self.__items) == 0

    def size(self):
        return len(self.__items)


def getpostfix(line):
    postfixstack = Stack()
    expression = ''
    expr = line.strip().replace(' ', '')  # gettin rid of the spaces

    for x in expr:
        if x.isnumeric():
            expression += str(x)
        elif x == '(':
            postfixstack.push(x)
        elif x == ')':
            d = postfixstack.pop()
            while d != '(':
                expression += str(d)
                d = postfixstack.pop()
        else:  # place operators in stack
            while (not postfixstack.is_empty()
                   ) and precedence[postfixstack.peek()] >= precedence[x]:
                expression += str(postfixstack.pop())
                postfixstack.push(x)

    # get stack into expression
    while postfixstack.is_empty() == False:
        expression += str(postfixstack.pop())
        print('postfix:', expression)
        return expression


def evaluatepostfix(postfix_expr):
    resultstack = Stack()
    # newresult = True #so x loops through all lines

    for x in postfix_expr:
        if x.isnumeric():
            resultstack.push(x)
        else:
            b = int(resultstack.pop())
            a = int(resultstack.pop())

            resultstack.push(eval('a'+x+'b'))
            # if newresult:
            print('result:', resultstack.peek())


# initialize
# content = input('Enter file name :')
if len(sys.argv) > 1:
    content = argv[1]
else:
    content = 'orig_input.txt'
print(content)
input_file_name = content
input_file = open(input_file_name, 'r')
precedence = {'*': 3, '/': 3, '%': 3, '+': 2, '-': 2, '(': 1}

# iterate ( run through each line) in the file and get postfix and then
# evaluate it
for line in input_file:
    postfix_expr = getpostfix(line)
    evaluatepostfix(postfix_expr)
    input_file.close()
