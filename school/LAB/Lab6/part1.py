#!/usr/bin/env python3

from __future__ import print_function
from future.types import *
import sys
import getopt
import random
from timeit import repeat
# import subprocess
# import re


##############################################################################


class CircularQueue:
    """Simple implementation of a bounded circular queue."""

    def __init__(self, capacity):
        assert capacity >= 2, "Max capacity for a queue must be at least 2."
        self.__capacity = capacity
        self.__items = [None]*capacity
        self.__count = 0
        self.__head = 0
        self.__tail = 0

    def enqueue(self, item):
        """Places an item at the back of the queue."""
        assert self.__count < self.__capacity, "Queue is full!"
        self.__items[self.__tail] = item
        self.__tail = (self.__tail + 1) % self.__capacity
        self.__count += 1

    def dequeue(self):
        """Returns and removes the item at the front of the queue."""
        assert self.__count > 0, "Queue is empty!"
        item = self.__items[self.__head]
        self.__items[self.__head] = None
        self.__head = (self.__head + 1) % self.__capacity
        self.__count -= 1
        return item

    def peek(self):
        """Returns the item at the front of the queue without removing it, or
        None if the the queue is empty (ie no exception is raised).
        """
        if self.__count > 0:
            return self.__items[self.__head]
        else:
            return None

    def get_size(self):
        return self.__count

    def is_full(self):
        return self.__count == (self.__capacity - 1)

    def is_empty(self):
        return self.__count == 0

    def clear(self):
        self.__items = [None for _ in range(self.__capacity)]
        self.__count = self.__head = self.__tail = 0

    def debug_get_items(self):
        """Returns the full item list as a python list. FOR DEBUGGING ONLY."""
        return self.__items

    def pdiagnostics(self):
        s = self  # Sleezy way to make the print fit into two lines < 79 chars.
        print("%s, H=%d, T=%d, (%d/%d)" %
              (str(s.__items), s.__head, s.__tail, s.__count, s.__capacity))


# ----------------------------------------------------------------------------


class LousyQueue:
    def __init__(self, capacity):
        assert capacity >= 2, "Max capacity for a queue must be at least 2."
        self.__capacity = capacity
        self.__items = []
        self.__size = 0

    def enqueue(self, item):
        """Places an item at the back of the queue."""
        assert self.__size < self.__capacity, "Queue is full!"
        self.__items.append(item)
        self.__size += 1

    def dequeue(self):
        """Returns and removes the item at the front of the queue."""
        assert self.__size > 0, "Queue is empty!"
        item = self.__items.pop(0)
        self.__size -= 1
        return item

    def peek(self):
        """Returns the item at the front of the queue without removing it, or
        None if the the queue is empty (ie no exception is raised).
        """
        if self.__count > 0:
            return self.__items[0]
        else:
            return None

    def get_size(self):
        return len(self.__items), self.__size

    def is_full(self):
        return self.__size == (self.__capacity - 1)

    def is_empty(self):
        return self.__size == 0

    def clear(self):
        self.__items = []
        self.__size = 0

    def debug_get_items(self):
        """Returns the full item list as a python list. FOR DEBUGGING ONLY."""
        return self.__items

    def pdiagnostics(self):
        s = self  # Sleezy way to make the print fit into two lines < 79 chars.
        print("%s, (%d/%d)" %
              (str(s.__items), s.__size, s.__capacity))


##############################################################################
# Functions


def main(type=None, Qlen=500, numtests=10000):
    lst = [LousyQueue(Qlen), CircularQueue(Qlen)]
    d = ["non-circular", "circular"]

    items = get_items(Qlen)

    for queue in lst:
        fill_queue(queue, items)

    # print("Testing non-circular queue time to dequeue %d items, for %d runs." % (Qlen, numtests))
    # result = timeit("this=deepcopy(circ); for i in range(Qlen-1): print(i); this.dequeue()", setup="from copy import deepcopy;", globals=locals(), number=numtests)
    # print("Result was: ", result)

    # print("Testing circular queue time to dequeue %d items, for %d runs." % (Qlen, numtests))
    # result = timeit("for i in range(Qlen-1): this.dequeue()", setup="from copy import deepcopy; this=deepcopy(circ)", globals=locals(), number=numtests)
    # print("Result was: ", result)
    #       result = timeit("for i in range(Qlen-1): this.dequeue()", setup="from copy import deepcopy; this=deepcopy(queue)", globals=locals(), number=numtests)

    for i in range(2):
        from copy import deepcopy
        import gc
        queue = lst[i]
        print("Testing time for a %s queue to dequeue %d items, for %d runs."
              % (d[i], Qlen, numtests))

        result = 0
        result = repeat("for i in range(Qlen-1): this.dequeue()",
                        # setup="gc.enable(); this=deepcopy(queue)",
                        setup="this=deepcopy(queue)",
                        globals=locals(), number=1, repeat=numtests)

        total = sum(result)
        print("Result was: ", total, "sec.")


def get_items(Qlen):
    return [int(10000*random.random()) for _ in range(Qlen)]


def fill_queue(queue, items):
    for i in items:
        queue.enqueue(i)


##############################################################################
# Main


# Getopt handling always looks so messy.
def get_options():
    """Process a few commandline options because I'm far, far too lazy to
    interactively type a filename and the number of cards for every test run.
    """
    kwargs = {}
    options = ["ht:12l:n:", ["help", "type=", "len=", "num="]]
    valid_types = ["non-circular", "circular"]
    try:
        opts, args = getopt.getopt(sys.argv[1:], *options)
    except getopt.GetoptError as e:
        print(e)
        print()
        ShowUsage(options)
    else:
        for opt, optarg in opts:
            opt = opt.lstrip('-')
            if opt in ('h', "help"):
                ShowUsage(options)
            elif opt in ('t', "type"):
                if optarg in valid_types:
                    kwargs["type"] = optarg
                else:
                    print("Value for option %s must be one of %s." %
                          (opt, str(valid_types)), file=sys.stderr)
                    sys.exit(1)
            elif opt.isdigit() and int(opt) < len(valid_types):
                kwargs["type"] = valid_types[int(opt)]
            elif opt in ('l', "len"):
                kwargs["Qlen"] = optarg_to_int_or_die(opt, optarg)
            elif opt in ('n', "num"):
                kwargs["numtests"] = optarg_to_int_or_die(opt, optarg)

        return kwargs


def optarg_to_int_or_die(opt, optarg):
    if optarg.isnumeric():
        return int(optarg)
    else:
        print("Option '%s' requires a numeric argument." % opt)
        sys.exit(1)


def ShowUsage(options):
    """Obligatory service."""
    print("""\
Usage: %s -[%s] --[%s]
-h --help         Show this help and exit.
-t --type=[type]  Specify type.
-1 -2             Aliases for -tnon-circular and -tcircular
-l --len=[N]      Max capacity of tested queues.
-n --num=[N]      Number of tests to run."""
          % (sys.argv[0], options[0], ', '.join(options[1])))
    sys.exit()


if __name__ == "__main__":
    try:
        main(**get_options())
    except (EOFError, KeyboardInterrupt) as e:
        print("")
        sys.exit(3)
