#!/usr/bin/env python3

import sys

###############################################################################
# Class Definitions


class Expression:
    def __init__(self, in_paren=False, init_operand=None):
        self.operands = []
        self.optypes = []
        self.in_paren = in_paren
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

    def print_self(self):
        """The required method to print things all postfix-like. Note that
        this method doesn't actually print anything. Obviously."""
        optype_i = 0
        buf = ""
        if self.in_paren:
            buf += "("
        for n in range(0, len(self.operands)):
            op = self.operands[n]
            if isinstance(op, Expression):
                buf += op.print_self()
            else:
                buf += str(op)
            buf += " "
            if n >= 1:
                buf += "%s " % self.optypes[optype_i]
                optype_i += 1
        if self.in_paren:
            buf = "%s)" % buf[:-1]
        return buf

    def print_infix(self):
        """Because actually reading reverse Polish is, like, hard."""
        optype_i = 0
        buf = ""
        if self.in_paren:
            buf += "("
        for n in range(0, len(self.operands)):
            op = self.operands[n]
            if isinstance(op, Expression):
                buf += op.print_infix()
            else:
                buf += str(op)
            buf += ' '
            if n < len(self.optypes):
                buf += "%s " % self.optypes[optype_i]
                optype_i += 1
        if self.in_paren:
            buf = "%s)" % buf[:-1]
        return buf


###############################################################################
# Function Definitions


def parseline(line, in_paren=False):
    dash = token = False
    paren = 0
    buf = ""
    expr = Expression(in_paren)
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
                expr.add_operand(parseline(line[i+1:], True))
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
    operands = expr.operands.copy()
    optypes = expr.optypes.copy()

    while len(operands) > 1:
        maxbind = max([prec[i] for i in optypes])
        for x in range(0, len(optypes)):
            op = optypes[x]
            if prec[op] == maxbind:
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

    if SHOW_PROGRESS:
        print("calc:   %6.1f  %s  %6.1f  =>  %.2f" % (a, ch, b, ans))
    return str(ans)

###############################################################################
# Main


def main(file_name):
    with open(file_name, 'r') as fp:
        for line in [i.rstrip('\n') for i in fp]:
            expr = parseline(line)
            if PRINT_INFIX:
                buf = expr.print_infix()
            else:
                buf = expr.print_self()
            expr = solve(expr)
            print("%9.3f =  %s" % (float(expr.ans), buf))
            if SHOW_PROGRESS:
                print("")


enum_ops = ['+', '-', '*', '/', '%']
prec = {"*": 2, "/": 2, "%": 2, "+": 1, "-": 1}
FILE = "orig_input.txt"
files = []
PRINT_INFIX = True
SHOW_PROGRESS = False

if len(sys.argv) > 1:
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
            main(filename)
        except IOError:
            print("lrn2typ")
else:
    main(FILE)
