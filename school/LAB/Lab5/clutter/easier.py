#!/usr/bin/env python3

import sys

FILE = "orig_input.txt"

###############################################################################
# Class Definitions


class Expression:
    def __init__(self, in_paren=False, init_operand=None):
        self.operands = []
        self.optypes = []
        self.answer = None
        self.index = 0
        self.in_paren = in_paren
        if init_operand is not None:
            self.operands.append(init_operand)
            self.ans = init_operand
        else:
            self.ans = 0

    def add_operand(self, buf):
        if not isinstance(buf, Expression):
            if len(buf) > 0:
                self.operands.append(buf)
        else:
            self.operands.append(buf)

    def add_optype(self, operator):
        if len(operator) > 0:
            self.optypes.append(operator)
            self.index += 1

    def print_self(self):
        """The required method to print things all postfix-like."""
        x = 0
        if self.in_paren:
            buf = "("
        else:
            buf = ""
        for i in range(0, len(self.operands)):
            op = self.operands[i]
            if isinstance(op, Expression):
                buf += op.print_self()
            else:
                buf += str(op)

            buf += ' '
            if i >= 1:
                buf += self.optypes[x] + " "
                x += 1
        if self.in_paren:
            buf = buf[:-1] + ')'
        return buf

    def print_infix(self):
        """Because actually reading reverse Polish is, like, hard."""
        x = 0
        if self.in_paren:
            buf = "("
        else:
            buf = ""
        for i in range(0, len(self.operands)):
            op = self.operands[i]
            if isinstance(op, Expression):
                buf += op.print_infix()
            else:
                buf += str(op)

            buf += ' '
            if i < len(self.optypes):
                buf += self.optypes[x] + " "
                x += 1
        if self.in_paren:
            buf = buf[:-1] + ')'
        return buf

    def isdigit(self):
        return False

    def dump(self):
        print("================================================")
        print("OPERANDS: ", self.ret_ops())
        print("optypeS: ", self.optypes)
        print("ANSWER: ", self.ans)

    def ret_ops(self):
        buf = ""
        for op in self.operands:
            if isinstance(op, Expression):
                buf += op.ret_ops() + " "
            else:
                buf += "'%s' " % op
        return buf


###############################################################################
# Function Definitions


def parseline(line, in_paren=False):
    dash = False
    token = False
    paren = 0
    buf = ""
    expr = Expression(in_paren)
    for i in range(0, len(line)):
        ch = line[i]
        if not token:
            buf = ""
        if not paren:
            if ch.isnumeric():
                if dash:
                    buf = "-"
                    dash = False
                buf = buf + ch
                token = True
            elif ch == '(':
                paren += 1
                if dash:
                    expr.add_optype('-')
                    dash = False
                token = False
                expr.add_operand(buf)
                expr.add_operand(parseline(line[i+1:], True))
            elif ch == '-':
                if dash:
                    expr.add_optype('-')
                    token = False
                elif token:
                    expr.add_operand(buf)
                    expr.add_optype('-')
                    token = False
                    dash = False
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
    if len(expr.optypes) == 1:
        return do_solve(expr)
    operands = expr.operands.copy()
    optypes = expr.optypes.copy()
    maxbind = max([prec[i] for i in optypes])
    # minbind = min([prec[i] for i in optypes])
    # if maxbind != minbind:
    while len(operands) > 1:
        maxbind = max([prec[i] for i in optypes])
        # minbind = min([prec[i] for i in optypes])
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
    # else:
        # return do_solve(expr)


def do_solve(expr):
    n = 1
    for cur_op in expr.optypes:
        if n == 1:
            expr.ans = dumb_eval(cur_op, get_val(expr.operands[n-1]), get_val(expr.operands[n]))
        else:
            expr.ans = dumb_eval(cur_op, get_val(expr.ans), get_val(expr.operands[n]))
        n += 1
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
    return str(ans)

###############################################################################
# Main


def main(file_name):
    with open(file_name, 'r') as fp:
        for line in [i.rstrip('\n') for i in fp]:
            expr = parseline(line)
            # buf = expr.print_self()
            buf = expr.print_infix()
            expr = solve(expr)
            print("%9.2f =  %s" % (float(expr.ans), buf))


enum_ops = ['+', '-', '*', '/', '%']
prec = {"*": 2, "/": 2, "%": 2, "+": 1, "-": 1}

if len(sys.argv) > 1:
    for filename in sys.argv[1:]:
        main(filename)
else:
    main(FILE)
