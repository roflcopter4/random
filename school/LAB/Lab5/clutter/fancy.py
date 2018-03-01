#!/usr/bin/env python3

import sys
# import copy


###############################################################################
# Class Definitions


class Expression:
    def __init__(self, in_paren=False, init_operand=None):
        self.operands = []
        self.optypes = []
        self.in_paren = in_paren
        self.color_op = None
        if init_operand is not None:
            self.operands.append(init_operand)
            self.ans = init_operand
        else:
            self.ans = 0

    def add_operand(self, operand):
        if not isinstance(operand, Expression):
            if len(operand) > 0:
                self.operands.append(operand)
        else:
            self.operands.append(operand)

    def add_optype(self, operator):
        if len(operator) > 0:
            self.optypes.append(operator)

    def print_infix(self, precision=None, color_all=False):
        """Because actually reading reverse Polish is, like, hard."""
        optype_i = 0
        buf = ""
        if self.in_paren:
            buf += "("
        for n in range(0, len(self.operands)):
            op = self.operands[n]
            color = (self.color_op is not None) and (self.color_op >= 0) and \
                (self.color_op == n or self.color_op == n-1)
            if isinstance(op, Expression):
                buf += op.print_infix(precision, color)
            else:
                if precision is None or float(int(float(op))) == float(op):
                    tmp = str(op)
                else:
                    tmp = "%.*f" % (precision, float(op))
                if color:
                    buf += "\033[32m%s\033[0m" % tmp
                else:
                    buf += tmp

            buf += " "

            if PRINT_INFIX:
                if n < len(self.optypes):
                    if (self.color_op is not None) and (self.color_op == n):
                        buf += "\033[1m\033[32m%s\033[0m " % self.optypes[optype_i]
                    else:
                        buf += "%s " % self.optypes[optype_i]
                    optype_i += 1
            else:
                if n >= 1:
                    if (self.color_op is not None) and (self.color_op == n):
                        buf += "\033[1m\033[32m%s\033[0m " % self.optypes[optype_i]
                    else:
                        buf += "%s " % self.optypes[optype_i]
                    optype_i += 1

        if self.in_paren:
            buf = "%s)" % buf[:-1]

        if self.color_op is not None:
            if color_all and self.color_op == -1:
                buf = "\033[31m%s\033[0m" % buf
            elif color_all:
                buf = "\033[32m%s\033[0m" % buf

            if self.color_op == -1:
                buf = "\033[33m%s\033[0m" % buf
                self.color_op = None
            else:
                self.color_op = -1
        elif color_all:
            buf = "\033[32m%s\033[0m" % buf
        return buf


###############################################################################
# Function Definitions


def parse_file(file_name):
    with open(file_name, 'r') as fp:
        for line in [i.rstrip('\n') for i in fp]:
            global GLOB_EXPR
            GLOB_EXPR = None
            expr = parse_line(line)
            buf = expr.print_infix()
            expr = solve(expr)
            print("%9.3f =  %s" % (float(expr.ans), buf))
            if SHOW_PROGRESS:
                print("")


def parse_line(line, in_paren=False):
    expr = Expression(in_paren)
    dash = token = False
    paren = 0
    buf = ""
    global GLOB_EXPR
    if GLOB_EXPR is None:
        GLOB_EXPR = expr

    for i in range(0, len(line)):
        ch = line[i]
        if not token:
            buf = ""
        if paren == 0:
            if ch.isnumeric():
                if dash:
                    buf = "-"
                    dash = False
                buf += ch
                token = True
            elif ch == '(':
                paren += 1
                if dash:
                    expr.add_optype('-')
                    dash = False
                token = True
                expr.add_operand(parse_line(line[i+1:], True))
            elif ch == '-':
                if dash:
                    expr.add_optype('-')
                    token = False
                elif token:
                    expr.add_operand(buf)
                    expr.add_optype('-')
                    dash = token = False
                else:
                    dash = True
            elif ch == ')':
                expr.add_operand(buf)
                return expr
            elif enum_ops.count(ch):
                if token:
                    expr.add_operand(buf)
                    token = False
                expr.add_optype(ch)
        elif ch == ')':
                paren -= 1
        elif ch == '(':
                paren += 1
    if token:
        expr.add_operand(buf)
    return expr


def get_val(operand):
    if isinstance(operand, Expression):
        return operand.ans
    else:
        return operand


def solve(expr):
    for op in expr.operands:
        if isinstance(op, Expression):
            op = solve(op)
    # operands = expr.operands.copy()
    # optypes = expr.optypes.copy()
    # expr2 = copy.deepcopy(expr)
    # operands = expr2.operands
    # optypes = expr2.optypes
    operands = expr.operands
    optypes = expr.optypes

    while len(operands) > 1:
        maxbind = max([prec[i] for i in optypes])
        for x in range(0, len(optypes)):
            op = optypes[x]
            if prec[op] == maxbind:
                if SHOW_PROGRESS:
                    expr.color_op = x
                    print(GLOB_EXPR.print_infix(2))
                a = operands.pop(x)
                b = operands.pop(x)
                optypes.pop(x)
                operands.insert(x, dumb_eval(op, get_val(a), get_val(b)))
                break
    expr.ans = operands[0]
    return expr


def dumb_eval(ch, a, b):
    a = float(a)
    b = float(b)
    ans = None
    if ch == '+':
        ans = a + b
    elif ch == '-':
        ans = a - b
    elif ch == '*':
        ans = a * b
    elif ch == '/':
        ans = a / b
    elif ch == '%':
        ans = a % b
    else:
        print("PANIC")
        sys.exit()

    # if SHOW_PROGRESS:
        # print("calc:   %6.1f  %s  %6.1f  =>  %.2f" % (a, ch, b, ans))
    return str(ans)


# def print_tmp(operands, optypes):
    # expr = Expression()

###############################################################################
# Main


FILE = "orig_input.txt"
PRINT_INFIX = True
SHOW_PROGRESS = False
GLOB_EXPR = None

enum_ops = ['+', '-', '*', '/', '%']
prec = {"*": 2, "/": 2, "%": 2, "+": 1, "-": 1}

if len(sys.argv) > 1:
    files = []
    for arg in sys.argv[1:]:
        if arg == '-p':
            PRINT_INFIX = False
            INIT = 2
        elif arg == '-v':
            SHOW_PROGRESS = True
        else:
            files.append(arg)

    for filename in files:
        try:
            parse_file(filename)
        except IOError:
            print("lrn2typ")
else:
    parse_file(FILE)
