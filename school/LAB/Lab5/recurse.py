#!/usr/bin/env python3
"""Parses and evaluates a mathematical expression, observing proper order of
operations. Prints the result and a cleaned up (ie spaces between each number
and operator) form of the expression. Apparently this is not at all what the
lab actually meant by a postfix expression - it should not have parentheses and
should be entirely stack based. Because of this fact the program prints the
expression in normal infix format by default. Use the '-p' flag to override.
The '-v' flag shows the status of the calculation for each step, higlighting
the currently evaluated sub-expression in green.
"""

import sys

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

    def format_line(self, precision=None, color_all=False):
        """Create and return the entire expression, neatly formatted, including
        the original parentheses. Loop through each operand, either printing if
        it is a simple number or recursively calling this method if it is an
        instance of the Expression class. The methods to format operands and
        operators (optypes) are split off to keep this a little bit neater.
        """
        optype_i = 0
        buf = ""
        if len(self.operands) == 1:  # If only one operand dont add parentheses
            self.in_paren = False
        if self.in_paren:
            buf += "("

        for n in range(0, len(self.operands)):
            op = self.operands[n]
            color = (self.color_op is not None) and \
                    (self.color_op == n or self.color_op == n-1)
            # Either format operand or recurse.
            if isinstance(op, Expression):
                buf += op.format_line(precision, color)
            else:
                buf += self.format_operand(op, precision, color)
            if len(self.operands) > 1:
                buf += " "
            # Format operator if appropriate.
            tmp = self.format_optype(optype_i, n)
            if tmp is not None:
                buf += tmp
                optype_i += 1

        # Add closing parentheses and color if appropriate.
        if self.in_paren:
            buf = "%s)" % buf[:-1]
        if color_all:
            buf = "\033[32m" + buf + "\033[0m"
        self.color_op = None
        return buf

    def format_operand(self, op, precision, color):
        if precision is None or float(int(float(op))) == float(op):
            buf = str(op)
        else:
            buf = "%.*f" % (precision, float(op))
        if color:
            buf = "\033[32m" + buf + "\033[0m"
        return buf

    def format_optype(self, index, n):
        buf = None
        if (PRINT_INFIX and n < len(self.optypes)) or (not PRINT_INFIX
                                                       and n >= 1):
            buf = str(self.optypes[index])
        if buf is not None:
            if (self.color_op is not None) and (self.color_op == n):
                buf = "\033[1m\033[32m" + buf + "\033[0m"
            buf += " "
        return buf


###############################################################################
# Function Definitions


def parse_file(fp):
    """Main loop for each input file."""
    for line in [i.rstrip('\n') for i in fp]:
        if SHOW_PROGRESS:
            global GLOBAL_EXPR
            GLOBAL_EXPR = None
        expr = parse_line(line)
        buf = expr.format_line()
        solve(expr)
        print("%9.3f =  %s" % (float(expr.ans), buf))
        if SHOW_PROGRESS:
            print("")


def parse_line(line, in_paren=False):
    """Steps through each line one character at a time. Recursively calls this
    function for each parenthetical sub-expression.
    'dash'  stores whether the previous character was a '-' in order to resolve
            the ambiguity between negation and subtraction.
    'token' stores whether the previous character(s) were numbers or
            sub-expressions rather than operators or ignored characters.
    'buf'   holds the current token including any minus signs, and is converted
            to an operand when the token ends.
    'paren' holds the current level of parentheses - calculations continue only
            when it is zero.
    """
    dash = token = False
    expr = Expression(in_paren)
    paren = 0
    buf = ""
    if SHOW_PROGRESS:
        global GLOBAL_EXPR
        if GLOBAL_EXPR is None:
            GLOBAL_EXPR = expr

    for i in range(0, len(line)):
        ch = line[i]
        if not token:  # Empty the buffer after adding the token.
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
                token = True  # Sub-expressions are tokens
                # Recurse starting from the next character (or else it will
                # immediately recurse again infinately many times).
                expr.add_operand(parse_line(line[i+1:], True))
            elif ch == '-':
                if dash:  # Should not actually be reachable but I'm paranoid.
                    expr.add_optype('-')
                    token = False
                elif token:  # End of token -> must be a subtraction operator.
                    expr.add_operand(buf)
                    expr.add_optype('-')
                    dash = token = False
                else:
                    dash = True
            elif ch == ')':
                break
            elif enum_ops.count(ch):  # Add a normal operator
                if token:
                    expr.add_operand(buf)
                    token = False
                expr.add_optype(ch)

        # Skip over characters already evaluated by sub expressions. Increment
        # and decrement paren when opening and closing parentheses are found
        # until paren is 0 and the parenthetical is thus skipped.
        elif ch == ')':
                paren -= 1
        elif ch == '(':
                paren += 1

    # Be sure to add any token that remains when breaking or leaving the loop.
    if token:
        expr.add_operand(buf)
    return expr


def solve(expr):
    """Recursively solves each sub expression until either only numbers remain
    or the sub expression has already been solved. Continue operations until
    only one operator remains - the answer.
    """
    for op in expr.operands:
        if isinstance(op, Expression):
            solve(op)
    operands = expr.operands
    optypes = expr.optypes

    # Find the value of the highest precidence operator in the expression, then
    # find the first instance of an expression with that precidence. Pop the
    # two numbers it applies to and calculate the answer. Continue until done.
    while len(operands) > 1:
        maxprec = max([precidence[i] for i in optypes])
        for x in range(0, len(optypes)):
            op = optypes[x]
            if precidence[op] == maxprec:
                if SHOW_PROGRESS:
                    expr.color_op = x
                    print(GLOBAL_EXPR.format_line(2))

                # Remember to remove the operator from the list
                optypes.pop(x)
                a = operands.pop(x)
                b = operands.pop(x)
                operands.insert(x, safe_eval(op, a, b))
                break

    # Sometimes the last operand turns out to be an Expression. In that case
    # make certain it is solved and set the answer to its answer.
    if isinstance(operands[0], Expression):
        if operands[0].ans is None:
            solve(operands[0])
        expr.ans = operands[0].ans
    else:
        expr.ans = operands[0]


def safe_eval(op, a, b):
    a = float(get_val(a))
    b = float(get_val(b))
    if op == "^":
        op = "**"
    return eval("a" + op + "b")


def get_val(operand):
    """If the operand is a simple number, return it. If it is an Expression
    object, return its answer attribute. NOTE: Should probably be a method.
    """
    if isinstance(operand, Expression):
        return operand.ans
    else:
        return operand


###############################################################################
# Main


FILE = "orig_input.txt"
PRINT_INFIX = True
SHOW_PROGRESS = False
GLOBAL_EXPR = None

# List of valid operations and a dictionary of their precidence values.
precidence = {"^": 3, "*": 2, "/": 2, "%": 2, "+": 1, "-": 1}
enum_ops = list(precidence.keys())
files = []

# Simple and lazy checking for any flags.
if len(sys.argv) > 1:
    for arg in sys.argv[1:]:
        if arg == '-p':
            PRINT_INFIX = False
            INIT = 2
        elif arg == '-v':
            SHOW_PROGRESS = True
        else:
            files.append(arg)
else:
    print("Usage: %s -[vp] <-> <files>..." % sys.argv[0] + """
-:  Read from the standard input.
-v: Print the status of the full expression after each calculation with the
    current sub-expression highlighted in green
-p: Print in semi-postfix mode.""")

for filename in files:
    if filename == "-":
        parse_file(sys.stdin)
    else:
        try:
            with open(filename, 'r') as fp_global:
                parse_file(fp_global)
        except IOError as errno:
            print("lrn2typ ->", errno)
