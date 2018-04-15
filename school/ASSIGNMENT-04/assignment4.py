#!/usr/bin/env python3
"""Creates a tree represeting all the possible game states for a game of Nim
given a starting pile size.
"""

import sys
from copy import copy

###############################################################################


class Cache:
    """Stores a list of all unique game states with as small a memory footprint
    as possible. The 'state' attribute of all nodes is a reference to a list
    stored in the (essentially) global instance of Cache.
    """

    def __init__(self):
        self.__cache = []

    def check_cache(self, state):
        """Compares a given game state to every entry in cache. Returns it if
        not found, otherwise returns a reference to the previously cached data.
        """
        for entry in self.__cache:
            if state == entry:
                return entry
        self.__cache.append(state)
        return state

    def get_num_items(self):
        """Returns the number of items in cache."""
        return len(self.__cache)


class Minimax:
    """Represents a node in the minimax tree."""
    COLORS = {"MAX": "\033[32m", "MIN": "\033[31m"}
    LEAF_ONLY = False
    cache = Cache()

    def __init__(self, nimState, minMaxLevel):
        self.state = self.cache.check_cache(nimState)
        self.level = minMaxLevel
        self.child = []

    def __swap_min_max(self):
        """Swaps 'MAX' to 'MIN' and vice versa."""
        return "MAX" if (self.level == "MIN") else "MIN"

    def __color_level(self):
        """Adds some flair to the output."""
        return "\033[%dm%s%s\033[0m" % \
            (self.LEAF_ONLY, self.COLORS[self.level], self.level)

    def solve(self):
        """Recursively creates the tree of all possible game states."""
        all_splits = self.split(self.state)

        for i, combinations in enumerate(all_splits):
            if combinations is not None:
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
            if (self.LEAF_ONLY and nchild == 0) or (not self.LEAF_ONLY):
                line += "\\-%s %s" % (self.state, self.__color_level())
            else:
                line += "\\-%s" % self.state
            indent += "  "

        else:
            line += "+ %s" % self.state
            indent += "| "

        print(line)

        for i in range(nchild):
            last = (nchild == i + 1)
            self.child[i].display(copy(indent), last)

    #
    # I'm not sure how people feel about goto statements nowadays (because
    # that's basically what 'raise Done' is), but this just seems like the most
    # straightforward way to break out of two nested loops.
    #
    # Also, I append None to the list if the values are less than or equal to 2
    # instead of the number or another list because those values can't be split
    # and will be skipped in solve() one way or another. It seems pointless to
    # add them to the return list if they won't be used anyway. Adding None
    # just functions as a placeholder so the iterator in solve() is incremented
    # but nothing is done. I just thought I'd mention this because it means
    # that this function does not actually return the same results as mentioned
    # in the assignment description. I really hope that is an ok sort of change
    # to make.
    #
    def split(self, state):
        """Returns all possible non-equal splits for each pile in a state list.
        Piles that are less than or equal to two can't be split and are
        replaced by None in the return list.
        """
        all_splits = []

        for pile in state:

            if pile <= 2:
                # The continue statement just keeps the nesting down a bit.
                all_splits.append(None)
                continue

            combinations = []
            # This is an incredibly stupid way to do this, but it's honestly
            # all I could think of. At least the goto equivalent let's us break
            # out halfway through our braindead n^2 double loop.
            try:
                for val1 in range(1, pile):
                    for val2 in range(1, pile):  # <- break

                        if (val1 + val2 == pile):
                            if (val1 >= val2):
                                raise Done
                            else:
                                combinations.append([])
                                combinations[-1].append(val1)
                                combinations[-1].append(val2)
                                # There can't possibly be more than one match
                                break
            except Done:
                pass
            all_splits.append(combinations)

        return all_splits


class Done(Exception):
    """Dummy wrapper class."""


###############################################################################


def main(init_tokens=None, quiet=False, leaf_only=False):
    """int argc, char **argv"""
    init_tokens = get_init_tokens(init_tokens)
    print("Solving for %d tokens." % init_tokens)

    Minimax.LEAF_ONLY = leaf_only
    root = Minimax([init_tokens], "MAX")

    root.solve()

    if quiet:
        print("There are %d items in cache." % root.cache.get_num_items())
    else:
        root.display()


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
    elif (not val.isnumeric()) or (int(val) <= 2):
        print("Command line argument is invalid.")
        val = input(message)

    while (not val.isnumeric()) or (int(val) <= 2):
        val = input("Invalid input. Please try again.  > ")

    return int(val)


###############################################################################


def get_options():
    """Process the few arguments to this program. Maybe someday I'll bother to
    learn how to use the argparse module.
    """
    import getopt
    options = ["hql", ["help", "quiet", "leaf-only"]]
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
                kwargs['leaf_only'] = True

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
 -q --quiet      Do not display tree (useful for benchmarking).
 -l --leaf-only  Only print 'min' or 'max' for leaf nodes, rather than do as
                 specified in the requirements for this assignment.""")
    sys.exit(status)


if __name__ == "__main__":
    main(**get_options())
