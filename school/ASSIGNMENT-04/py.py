#!/usr/bin/env python3
"""Creates a tree represeting all the possible game states for a game of Nim
given a starting pile size.
"""

import sys
from copy import copy

LEAF_ONLY = False
# The only reason this is global is to keep memory usage down. No reason to
# have it duplicated for every single node.
COLORS = {"MAX": "\033[32m", "MIN": "\033[31m"}

###############################################################################


class Done(Exception):
    """Dummy wrapper class."""


class Minimax:
    """Represents a node in the minimax tree."""

    def __init__(self, nimState, minMaxLevel):
        self.state = nimState
        self.level = minMaxLevel
        self.child = []

    def __swap_min_max(self):
        """Swaps 'MAX' to 'MIN' and vice versa."""
        return "MAX" if (self.level == "MIN") else "MIN"

    def __color_level(self):
        """Adds some flair to the output."""
        return "\033[1m%s%s\033[0m" % (COLORS[self.level], self.level)

    def solve(self):
        """Recursively creates the tree of all possible game states."""
        for i in range(len(self.state)):
            pile = self.state[i]
            if pile > 2:
                combinations = split(pile)
                for combo in combinations:
                    new_state = copy(self.state)
                    new_state[i] = combo[0]
                    new_state = sorted(new_state + combo[1:])

                    node = Minimax(new_state, self.__swap_min_max())
                    self.child.append(node)
                    node.solve()

    def display(self, indent=" ", last=True):
        """Recursively steps through the tree and displays it in a manner
        resembling the outuput of tree(1).
        """
        nchild = len(self.child)
        line = indent
        if last:
            if LEAF_ONLY:
                if nchild == 0:
                    line += "\\-%s %s" % (self.state, self.__color_level())
                else:
                    line += "\\-" + str(self.state)
            else:
                line += "\\-%s %s" % (self.state, self.__color_level())
            indent += "  "
        else:
            line += "+ " + str(self.state)
            indent += "| "
        print(line)

        for i in range(nchild):
            last = (nchild == i + 1)
            self.child[i].display(copy(indent), last)


class Cache:
    def __init__(self):
        pass


###############################################################################


def main(init_tokens=None, quiet=False):
    init_tokens = get_init_tokens(init_tokens)
    print("Solving for %d tokens." % init_tokens)

    root = Minimax([init_tokens], "MAX")
    root.solve()
    if not quiet:
        root.display()


def split(pile):
    """Returns all possible non-equal splits for a given pile. I made this a
    function because pylint whines about not using 'self' if it is a method.
    """
    combinations = []
    comb_range = range(1, pile)
    try:
        for val1 in comb_range:
            for val2 in comb_range:
                if val1 + val2 == pile:
                    if val1 >= val2:
                        raise Done  # This is basically a goto
                    else:
                        combinations.append([])
                        combinations[-1].append(val1)
                        combinations[-1].append(val2)
                        break  # There can't possibly be more than one match
    except Done:
        pass
    # Pylint whines about inconsistent return statements if I connect the
    # return to the except block.
    return combinations


def get_init_tokens(val=None):
    """Either prompts the user for a value or analyzes the one provided as an
    argument. If either is not an integer greater than 2, the user is prompted
    again until a correct value is given, which is returned.
    """
    message = """\
Please enter your desired initial pile size. Input must be an integer > 2.
%sWARNING:%s Values greater than 17 will almost certainly use all the RAM on
your system and likely never terminate.\n> """ % ("\033[1m\033[31m", "\033[0m")

    if val is None:
        val = input(message)
    elif not val.isnumeric() or int(val) <= 2:
        print("Command line argument is invalid.")
        val = input(message)

    while not val.isnumeric() or int(val) <= 2:
        val = input("Invalid input. Please try again.  > ")

    return int(val)


###############################################################################


def get_options():
    """Process the few arguments to this program."""
    import getopt
    options = ["hVql", ["help", "quiet", "leaf-only"]]
    kwargs = {}
    try:
        opts, args = getopt.gnu_getopt(sys.argv[1:], *options)
    except getopt.GetoptError as err:
        print(err, '\n')
        show_usage(2)
    else:
        for opt, optarg in opts:
            opt = opt.lstrip('-')
            if opt in ('h', "help"):
                show_usage(0)
            elif opt in ('q', "quiet"):
                kwargs['quiet'] = True
            elif opt in ('l', "LEAF_ONLY"):
                global LEAF_ONLY
                LEAF_ONLY = True
        if args:
            kwargs['init_tokens'] = args[0]

        return kwargs


def show_usage(status):
    """Obligatory service. I miss perl's heredocs."""
    from os.path import basename
    print("Usage: %s [options] <initial size>" % basename(sys.argv[0]))
    print("""
Initial state indicates the size of the inital pile for the game of Nim. The
user will be prompted for its value interactively if either no value or an
invalid value is provided.

Options
 -h --help       Show this usage information.
 -q --quiet      Do not display tree (useful for benchmarking)
 -l --leaf-only  Only print 'min' or 'max' for leaf nodes, rather than do as
                 specified in the requirements for this assignment""")
    sys.exit(status)


if __name__ == "__main__":
    main(**get_options())
